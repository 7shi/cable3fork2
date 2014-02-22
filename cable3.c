#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#ifdef _WIN32
#include <conio.h>
#endif

#define ROMBASE 0xf0000
uint8_t mem[0x200000 /* 2MB */ ], ioport[0x10000];
uint16_t r[8], IP;
uint8_t *const r8 = (uint8_t *) r;
uint8_t CF, PF, AF, ZF, SF, TF, IF, DF, OF;
uint8_t ptable[256];
int oprsz;
FILE *files[3];			/* HD, FD, BIOS */

#define AL r8[0]
#define AH r8[1]
#define CL r8[2]
#define CH r8[3]
#define DL r8[4]
#define DH r8[5]
#define BL r8[6]
#define BH r8[7]

#define AX r[0]
#define CX r[1]
#define DX r[2]
#define BX r[3]
#define SP r[4]
#define BP r[5]
#define SI r[6]
#define DI r[7]

struct SReg {
	uint8_t *p;
	uint16_t v;
};
struct SReg sr[4];

#define ES sr[0]
#define CS sr[1]
#define SS sr[2]
#define DS sr[3]

void
debug(FILE * f)
{
	fprintf(f,
	    "%04x %04x %04x %04x-%04x %04x %04x %04x %c%c%c%c%c%c%c%c%c %04x %04x %04x %04x:%04x %02x%02x%02x\n",
	    AX, BX, CX, DX, SP, BP, SI, DI,
	    "-O"[OF], "-D"[DF], "-I"[IF], "-T"[TF], "-S"[SF], "-Z"[ZF], "-A"[AF], "-P"[PF], "-C"[CF],
	    ES.v, SS.v, DS.v, CS.v, IP, CS.p[IP], CS.p[IP + 1], CS.p[IP + 2]);
}

static inline void
setsr(struct SReg * sr, uint16_t v)
{
	sr->p = &mem[(sr->v = v) << 4];
}

static inline uint16_t
read16(void *p)
{
	return *(uint16_t *) p;
}

static inline void
write32(void *p, uint32_t v)
{
	*(uint32_t *) p = v;
}

static inline int
getv(void *src)
{
	return oprsz ? *(uint16_t *) src : *(uint8_t *) src;
}

static inline int
setv(void *dst, int src)
{
	if (oprsz)
		return *(uint16_t *) dst = src;
	return *(uint8_t *) dst = src;
}

static inline int
setvp(void *dst, void *src)
{
	if (oprsz)
		return *(uint16_t *) dst = *(uint16_t *) src;
	return *(uint8_t *) dst = *(uint8_t *) src;
}

static inline void
push(uint16_t src)
{
	*(uint16_t *) &SS.p[SP -= 2] = src;
}

static inline uint16_t
pop(void)
{
	uint16_t ret = *(uint16_t *) &SS.p[SP];
	SP += 2;
	return ret;
}

static inline uint8_t *
regmap(int reg)
{
	return &r8[oprsz ? 2 * reg : (2 * reg + reg / 4) & 7];
}

static inline int
isneg(int v)
{
	return (oprsz ? (int16_t) v : (int8_t) v) < 0;
}

static inline void
setafof(uint16_t src, uint16_t dst, uint16_t val)
{
	src ^= dst ^ val;
#ifdef USE_AF
	AF = !!(src & 16);
#endif
	OF = (val - dst && 1 & (CF ^ src >> (8 * (oprsz + 1) - 1)));
}

static inline void
setsfzfpf(uint16_t val)
{
	SF = isneg(val);
	ZF = !val;
	PF = ptable[(uint8_t) val];
}

static inline uint16_t
getflags(void)
{
	return 0xf002 | CF | (PF << 2) | (AF << 4) | (ZF << 6) | (SF << 7) | (TF << 8) | (IF << 9) | (DF << 10) | (OF << 11);
}

static inline void
setflags(uint16_t flags)
{
	CF = flags & 1;
	PF = (flags & 4) != 0;
	AF = (flags & 16) != 0;
	ZF = (flags & 64) != 0;
	SF = (flags & 128) != 0;
	TF = (flags & 256) != 0;
	IF = (flags & 512) != 0;
	DF = (flags & 1024) != 0;
	OF = (flags & 2048) != 0;
}

static inline void
intr(int n)
{
	push(getflags());
	push(CS.v);
	push(IP);
	setsr(&CS, *(uint16_t *) &mem[4 * n + 2]);
	IP = *(uint16_t *) &mem[4 * n];
	IF = TF = 0;
}

static inline uint8_t *
modrm(int *oprlen, int mode, int rm, int16_t disp, uint8_t *segpfx)
{
	if (mode == 3) {
		*oprlen = 0;
		return regmap(rm);
	}
	*oprlen = mode;
	uint16_t addr;
	struct SReg *seg;
	switch (rm) {
	case 0:
		seg = &DS, addr = BX + SI;
		break;
	case 1:
		seg = &DS, addr = BX + DI;
		break;
	case 2:
		seg = &SS, addr = BP + SI;
		break;
	case 3:
		seg = &SS, addr = BP + DI;
		break;
	case 4:
		seg = &DS, addr = SI;
		break;
	case 5:
		seg = &DS, addr = DI;
		break;
	case 6:
		if (mode == 0) {
			*oprlen += 2;
			seg = &DS, addr = disp;
		} else
			seg = &SS, addr = BP;
		break;
	case 7:
		seg = &DS, addr = BX;
		break;
	}
	if (mode)
		addr += disp;
	return (segpfx ? segpfx : seg->p) + addr;
}

static inline void
getoprs(int dir, int reg, uint8_t *addr, uint8_t **opr1, uint8_t **opr2)
{
	if (!dir)
		*opr1 = addr, *opr2 = regmap(reg);
	else
		*opr1 = regmap(reg), *opr2 = addr;
}

static inline void
jumpif(int8_t offset, int cond)
{
	IP += cond ? 2 + offset : 2;
}

static inline void
step(int rep, uint8_t *segpfx)
{
	uint8_t *p = &CS.p[IP], b = *p;
	oprsz = b & 1;
	int dir = b / 2 & 1;
	int rm = p[1] & 7, reg = p[1] / 8 & 7;
	int mode = p[1] >> 6;
	int16_t disp = mode != 1 ? read16(p + 2) : (int8_t) p[2];
	int opr;
	if (!(mode == 0 && rm == 6) && mode != 2) {
		if (mode != 1)
			opr = disp;
		else
			opr = (int16_t) read16(p + 3);
	} else
		opr = (int16_t) read16(p + 4);
	int oprlen;
	uint8_t *addr, *opr1, *opr2;
	getoprs(dir, reg, addr = modrm(&oprlen, mode, rm, disp, segpfx), &opr1, &opr2);
	uint16_t src, dst, val = 0;
	switch (b) {
		int tmp, tmp2;
		uint32_t utmp;
	case 0x70:		/* jo */
		return jumpif(p[1], OF);
	case 0x71:		/* jno */
		return jumpif(p[1], !OF);
	case 0x72:		/* jb, jnae */
		return jumpif(p[1], CF);
	case 0x73:		/* jnb, jae */
		return jumpif(p[1], !CF);
	case 0x74:		/* je, jz */
		return jumpif(p[1], ZF);
	case 0x75:		/* jne, jnz */
		return jumpif(p[1], !ZF);
	case 0x76:		/* jbe, jna */
		return jumpif(p[1], CF || ZF);
	case 0x77:		/* jnbe, ja */
		return jumpif(p[1], !CF && !ZF);
	case 0x78:		/* js */
		return jumpif(p[1], SF);
	case 0x79:		/* jns */
		return jumpif(p[1], !SF);
	case 0x7a:		/* jp */
		return jumpif(p[1], PF);
	case 0x7b:		/* jnp */
		return jumpif(p[1], !PF);
	case 0x7c:		/* jl, jnge */
		return jumpif(p[1], SF != OF);
	case 0x7d:		/* jnl, jge */
		return jumpif(p[1], SF == OF);
	case 0x7e:		/* jle, jng */
		return jumpif(p[1], ZF || SF != OF);
	case 0x7f:		/* jnle, jg */
		return jumpif(p[1], !ZF && SF == OF);
	case 0xb0:		/* mov r8, imm8 */
	case 0xb1:
	case 0xb2:
	case 0xb3:
	case 0xb4:
	case 0xb5:
	case 0xb6:
	case 0xb7:
	case 0xb8:		/* mov r16, imm16 */
	case 0xb9:
	case 0xba:
	case 0xbb:
	case 0xbc:
	case 0xbd:
	case 0xbe:
	case 0xbf:
		oprsz = b & 8;
		setvp(regmap(b & 7), p + 1);
		IP += oprsz ? 3 : 2;
		return;
	case 0x40:		/* inc */
	case 0x41:
	case 0x42:
	case 0x43:
	case 0x44:
	case 0x45:
	case 0x46:
	case 0x47:
	case 0x48:		/* dec */
	case 0x49:
	case 0x4a:
	case 0x4b:
	case 0x4c:
	case 0x4d:
	case 0x4e:
	case 0x4f:
		oprsz = 2, reg = b >= 0x48;
		opr1 = addr = modrm(&oprlen, mode, rm, disp, segpfx), opr2 = regmap(b & 7);
	case 0xfe:		/* inc, dec, call, callf, jmp, jmpf, push */
	case 0xff:
		if (reg < 2) {	/* inc, dec */
			val = setv(opr2, (dst = getv(opr2)) + (1 - 2 * reg));
			src = 1;
			setafof(src, dst, val);
			setsfzfpf(val);
			OF = isneg(dst) != isneg(val) && dst && val;
			if (oprsz == 2)
				++IP;
			else
				IP += 2 + oprlen;
		} else if (reg != 6) {
			IP += 2 + oprlen;
			if (reg == 3)	/* callf */
				push(CS.v);
			if (reg & 2)	/* call, callf */
				push(IP);
			if (reg & 1)	/* jmpf */
				setsr(&CS, *(uint16_t *) &opr2[2]);
			IP = *(uint16_t *) opr2;
		} else {
			push(*(uint16_t *) addr);
			IP += 2 + oprlen;
		}
		return;
	case 0x50:		/* push */
	case 0x51:
	case 0x52:
	case 0x53:
	case 0x54:
	case 0x55:
	case 0x56:
	case 0x57:
		push(r[b & 7]);
		++IP;
		return;
	case 0x58:		/* pop */
	case 0x59:
	case 0x5a:
	case 0x5b:
	case 0x5c:
	case 0x5d:
	case 0x5e:
	case 0x5f:
		r[b & 7] = pop();
		++IP;
		return;
	case 0xf6:		/* test, not, neg, mul, imul, div, idiv */
	case 0xf7:
		switch (reg) {
		case 0:	/* test */
			IP -= ~oprsz;
			setsfzfpf(getv(opr2) & opr);
			OF = CF = 0;
			break;
		case 2:	/* not */
			setv(opr2, ~getv(opr2));
			break;
		case 3:	/* neg */
			val = setv(opr2, -(dst = getv(opr2)));
			CF = val > (dst = 0);
			setafof(dst, dst, val);
			OF = dst == 0x8000;
			setsfzfpf(val);
			break;
		case 4:	/* mul */
			if (oprsz) {
				utmp = *(uint16_t *) addr * AX;
				DX = utmp >> 16;
				AX = utmp;
				OF = CF = DX != 0;
			} else {
				AX = utmp = *addr * AL;
				OF = CF = (utmp >> 8) != 0;
			}
			break;
		case 5:	/* imul */
			if (oprsz) {
				tmp = *(int16_t *) addr * (int16_t) AX;
				DX = tmp >> 16;
				AX = tmp;
				OF = CF = DX != 0;
			} else {
				AX = tmp = *(int8_t *) addr *(int8_t) AL;
				OF = CF = (tmp >> 8) != 0;
			}
			break;
		case 6:	/* div */
			if (oprsz) {
				if ((tmp2 = *(uint16_t *) addr)) {
					utmp = (uint32_t) (tmp = (DX << 16) + AX) / tmp2;
					if (!(utmp - (uint16_t) utmp))
						DX = tmp - tmp2 * (AX = utmp);
					else
						intr(0);
				}
			} else {
				if ((tmp2 = *addr)) {
					utmp = (uint16_t) (tmp = (AH << 16) + AX) / tmp2;
					if (!(utmp - (uint8_t) utmp))
						AH = tmp - tmp2 * (AL = utmp);
					else
						intr(0);
				}
			}
			break;
		case 7:	/* idiv */
			if (oprsz) {
				if ((tmp2 = *(int16_t *) addr)) {
					utmp = (int) (tmp = (DX << 16) + AX) / tmp2;
					if (!(utmp - (int16_t) utmp))
						DX = tmp - tmp2 * (AX = utmp);
					else
						intr(0);
				}
			} else {
				if ((tmp2 = *(int8_t *) addr)) {
					utmp = (int16_t) (tmp = (AH << 16) + AX) / tmp2;
					if (!(utmp - (int8_t) utmp))
						AH = tmp - tmp2 * (AL = utmp);
					else
						intr(0);
				}
			}
			break;
		}
		IP += 2 + oprlen;
		return;
	case 0x04:		/* add */
	case 0x05:
	case 0x0c:		/* or */
	case 0x0d:
	case 0x14:		/* adc */
	case 0x15:
	case 0x1c:		/* sbb */
	case 0x1d:
	case 0x24:		/* and */
	case 0x25:
	case 0x2c:		/* sub */
	case 0x2d:
	case 0x34:		/* xor */
	case 0x35:
	case 0x3c:		/* cmp */
	case 0x3d:
		addr = r8, opr = read16(p + 1), mode = 3, IP--, oprlen = 0;
	case 0x80:		/* add, or, adc, sbb, and, sub, xor, cmp */
	case 0x81:
	case 0x82:
	case 0x83:
		opr1 = addr, opr2 = (uint8_t *) &utmp;
		utmp = (dir |= !oprsz) ? (int8_t) opr : opr;
		IP -= ~!dir;
	case 0x00:		/* add */
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x08:		/* or */
	case 0x09:
	case 0x0a:
	case 0x0b:
	case 0x10:		/* adc */
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x18:		/* sbb */
	case 0x19:
	case 0x1a:
	case 0x1b:
	case 0x20:		/* and */
	case 0x21:
	case 0x22:
	case 0x23:
	case 0x28:		/* sub */
	case 0x29:
	case 0x2a:
	case 0x2b:
	case 0x30:		/* xor */
	case 0x31:
	case 0x32:
	case 0x33:
	case 0x38:		/* cmp */
	case 0x39:
	case 0x3a:
	case 0x3b:
	case 0x88:		/* mov */
	case 0x89:
	case 0x8a:
	case 0x8b:
		tmp = b >> 3;
		if (tmp == 16)
			tmp = reg;
		switch (tmp) {
		case 0:	/* add */
			val = setv(opr1, (dst = getv(opr1)) + (src = getv(opr2)));
			CF = val < dst;
			setafof(src, dst, val);
			break;
		case 1:	/* or */
			val = setv(opr1, getv(opr1) | getv(opr2));
			OF = CF = 0;
			break;
		case 2:	/* adc */
			val = setv(opr1, (dst = getv(opr1)) + (src = getv(opr2)) + CF);
			CF = (CF && val == dst) || +val < +(int) dst;
			setafof(src, dst, val);
			break;
		case 3:	/* sbb */
			val = setv(opr1, (dst = getv(opr1)) - (src = getv(opr2)) - CF);
			CF = (CF && val == dst) || -val < -(int) dst;
			setafof(src, dst, val);
			break;
		case 4:	/* and */
			val = setv(opr1, getv(opr1) & getv(opr2));
			OF = CF = 0;
			break;
		case 5:	/* sub */
			val = setv(opr1, (dst = getv(opr1)) - (src = getv(opr2)));
			CF = val > dst;
			setafof(src, dst, val);
			break;
		case 6:	/* xor */
			val = setv(opr1, getv(opr1) ^ getv(opr2));
			OF = CF = 0;
			break;
		case 7:	/* cmp */
			val = (dst = getv(opr1)) - (src = getv(opr2));
			CF = val > dst;
			setafof(src, dst, val);
			break;
		case 17:	/* mov */
			setvp(opr1, opr2);
			break;
		}
		if (b < 0x88)
			setsfzfpf(val);
		IP += 2 + oprlen;
		return;
	case 0x8c:		/* mov, lea, pop */
	case 0x8d:
	case 0x8e:
	case 0x8f:
		if (!oprsz) {
			oprsz = 1;
			addr = modrm(&oprlen, mode, rm, disp, segpfx);
			if (!dir)
				setv(addr, sr[reg].v);
			else
				setsr(&sr[reg], getv(addr));
		} else if (!dir)
			*(uint16_t *) opr2 = modrm(&oprlen, mode, rm, disp, mem) - mem;
		else
			*(uint16_t *) addr = pop();
		IP += 2 + oprlen;
		return;
	case 0xa0:		/* mov */
	case 0xa1:
	case 0xa2:
	case 0xa3:
		getoprs(dir, 0, modrm(&oprlen, 0, 6, read16(p + 1), segpfx), &opr1, &opr2);
		setvp(opr2, opr1);
		IP += 3;
		return;
	case 0xd0:		/* rol, ror, rcl, rcr, shl, sal, shr, sar */
	case 0xd1:
	case 0xd2:
	case 0xd3:
	case 0xc0:
	case 0xc1:
		utmp = isneg(dst = getv(addr));
		if (b < 0xd0) {
			++IP;
			tmp = (int8_t) disp;
		} else if (dir)
			tmp = 31 & CL;
		else
			tmp = 1;
		if (tmp) {
			if (reg < 4) {
				tmp %= reg / 2 + 8 * (oprsz + 1);
				setvp(&utmp, addr);
			}
			if (reg & 1)
				val = setv(addr, dst >> tmp);
			else
				val = setv(addr, dst << tmp);
			if (reg >= 5)
				CF = dst >> (tmp - 1) & 1;
		} else {
			val = dst;
		}
		switch (reg) {
		case 0:	/* rol */
			val = setv(addr, getv(addr) + (utmp >> (8 * (oprsz + 1) - tmp)));
			OF = isneg(val) ^ (CF = val & 1);
			break;
		case 1:	/* ror */
			utmp &= (1 << tmp) - 1;
			val = setv(addr, getv(addr) + (utmp << (8 * (oprsz + 1) - tmp)));
			CF = isneg(val);
			OF = isneg(val << 1) ^ CF;
			break;
		case 2:	/* rcl */
			val = setv(addr, getv(addr) + (CF << (tmp - 1)) + (utmp >> (1 + 8 * (oprsz + 1) - tmp)));
			CF = !!(utmp & 1 << (8 * (oprsz + 1) - tmp));
			OF = isneg(val) ^ CF;
			break;
		case 3:	/* rcr */
			val = setv(addr, getv(addr) + (CF << (8 * (oprsz + 1) - tmp)) + (utmp << (1 + 8 * (oprsz + 1) - tmp)));
			CF = !!(utmp & 1 << (tmp - 1));
			OF = isneg(val) ^ isneg(val << 1);
			break;
		case 4:	/* shl */
			CF = isneg(dst << (tmp - 1));
			OF = isneg(val) ^ CF;
			break;
		case 5:	/* shr */
			OF = isneg(dst);
			break;
		case 7:	/* sar */
			if (tmp >= 8 * (oprsz + 1))
				CF = !!utmp;
			OF = 0;
			val = setv(addr, getv(addr) + utmp * ~(((1 << (8 * (oprsz + 1))) - 1) >> tmp));
			break;
		}
		if (tmp && reg > 3) {
			setsfzfpf(val);
			IP += 2 + oprlen;
		} else
			IP += 2 + oprlen + (b < 0xd0);
		return;
	case 0xe0:		/* loopnz, loopne */
		return jumpif(p[1], --CX && !ZF);
	case 0xe1:		/* loopz, loope */
		return jumpif(p[1], --CX && ZF);
	case 0xe2:		/* loop */
		return jumpif(p[1], --CX);
	case 0xe3:		/* jcxz */
		return jumpif(p[1], !CX);
	case 0xe8:		/* call, jmp, jmpf */
	case 0xe9:
	case 0xea:
	case 0xeb:
		IP += 3 - dir;
		if (!oprsz) {
			if (dir) {
				setsr(&CS, read16(p + 3));
				IP = 0;
			} else
				push(IP);
		}
		IP += dir * oprsz ? (int8_t) p[1] : read16(p + 1);
		return;
	case 0x84:		/* test */
	case 0x85:
		setsfzfpf(getv(opr1) & getv(opr2));
		OF = CF = 0;
		IP += 2 + oprlen;
		return;
	case 0x90:		/* xchg */
	case 0x91:
	case 0x92:
	case 0x93:
	case 0x94:
	case 0x95:
	case 0x96:
	case 0x97:
		oprsz = 7, opr1 = r8, opr2 = regmap(b & 7);
	case 0x86:		/* xchg */
	case 0x87:
		if (opr1 != opr2) {
			tmp = getv(opr1);
			setv(opr1, getv(opr2));
			setv(opr2, tmp);
		}
		if (oprsz == 7)
			++IP;
		else
			IP += 2 + oprlen;
		return;
	case 0xa4:		/* movsb, movsw */
	case 0xa5:
	case 0xaa:		/* stosb, stosw */
	case 0xab:
	case 0xac:		/* lodsb,lodsw */
	case 0xad:
		if (!rep || CX) {
			tmp2 = (b >> 2) - 41;	/* 0, 1, 2 */
			tmp = ~(-2 * DF) * ~oprsz;
			do {
				opr1 = tmp2 < 2 ? &ES.p[DI] : r8;
				opr2 = tmp2 == 1 ? r8 : (segpfx ? segpfx : DS.p) + SI;
				setvp(opr1, opr2);
				if (tmp2 != 1)
					SI += tmp;
				if (tmp2 != 2) {
					DI += tmp;
				}
			} while (rep && --CX);
		}
		++IP;
		return;
	case 0xa6:		/* cmpsb, cmpsw */
	case 0xa7:
	case 0xae:		/* scasb, scasw */
	case 0xaf:
		if (!rep || CX) {
			tmp = ~(-2 * DF) * ~oprsz;
			do {
				opr1 = b >= 0xae ? r8 : (segpfx ? segpfx : DS.p) + SI;
				opr2 = &ES.p[DI];
				val = (dst = getv(opr1)) - (src = getv(opr2));
				ZF = !val;
				CF = val > dst;
				if (b < 0xae)
					SI += tmp;
				DI += tmp;
			} while (rep && --CX && ZF == rep - 1);
			setafof(src, dst, val);
			setsfzfpf(val);
		}
		++IP;
		return;
	case 0xc2:		/* ret */
	case 0xc3:
	case 0xca:		/* retf */
	case 0xcb:
	case 0xcf:		/* iret */
		dir = oprsz;
		IP = pop();
		if (b >= 0xca)	/* retf, iret */
			setsr(&CS, pop());
		if (b == 0xcf)	/* iret */
			setflags(pop());
		else if (!dir)
			SP += read16(p + 1);
		return;
	case 0xc6:		/* mov */
	case 0xc7:
		setv(opr2, opr);
		IP += 3 + oprlen + oprsz;
		return;
	case 0xe4:		/* in */
	case 0xe5:
		setvp(&AX, ioport + p[1]);
		IP += 2;
		return;
	case 0xec:		/* in */
	case 0xed:
		setvp(&AX, ioport + DX);
		++IP;
		return;
	case 0xe6:		/* out */
	case 0xe7:
		setvp(ioport + p[1], &AX);
		IP += 2;
		return;
	case 0xee:		/* out */
	case 0xef:
		setvp(ioport + DX, &AX);
		++IP;
		return;
	case 0xf2:		/* repnz, repz */
	case 0xf3:
		++IP;
		step(oprsz + 1, segpfx);
		return;
	case 0x06:		/* push */
	case 0x0e:		/* push */
	case 0x16:		/* push */
	case 0x1e:		/* push */
		push(sr[b >> 3].v);
		++IP;
		return;
	case 0x07:		/* pop */
	case 0x17:		/* pop */
	case 0x1f:		/* pop */
		setsr(&sr[b >> 3], pop());
		++IP;
		return;
	case 0x26:		/* es: */
	case 0x2e:		/* cs: */
	case 0x36:		/* ss: */
	case 0x3e:		/* ds: */
		++IP;
		step(rep, sr[(b >> 3) & 3].p);
		return;
	case 0x98:		/* cbw */
		AH = -isneg(AL);
		++IP;
		return;
	case 0x99:		/* cwd */
		DX = -isneg(AX);
		++IP;
		return;
	case 0x9a:		/* callf */
		push(CS.v);
		push(IP + 5);
		setsr(&CS, read16(p + 3));
		IP = read16(p + 1);
		return;
	case 0x9c:		/* pushf */
		push(getflags());
		++IP;
		return;
	case 0x9d:		/* popf */
		setflags(pop());
		++IP;
		return;
	case 0x9e:		/* sahf */
		setflags((getflags() & ~255) + AH);
		++IP;
		return;
	case 0x9f:		/* lahf */
		AH = getflags();
		++IP;
		return;
	case 0xc4:		/* les */
	case 0xc5:		/* lds */
		oprsz = 1;
		opr1 = regmap(reg), opr2 = modrm(&oprlen, mode, rm, disp, segpfx);
		setvp(opr1, opr2);
		if (b == 0xc4)
			setsr(&ES, *(uint16_t *) &opr2[2]);
		else
			setsr(&DS, *(uint16_t *) &opr2[2]);
		IP += 2 + oprlen;
		return;
	case 0xcc:		/* int3 */
		++IP;
		intr(3);
		return;
	case 0xcd:		/* int */
		IP += 2;
		intr(read16(p + 1) & 255);
		return;
	case 0xce:		/* into */
		++IP;
		if (OF)
			intr(4);
		return;
	case 0xd4:		/* aam */
		tmp = read16(p + 1);
		if (tmp &= 255) {
			AH = AL / tmp;
			val = AL %= tmp;
			setsfzfpf(val);
		} else
			intr(0);
		OF = CF = 0;
		IP += 2;
		return;
	case 0xd5:		/* aad */
		oprsz = 0, AX = val = AL + read16(p + 1) * AH;
		setsfzfpf(val);
		OF = CF = 0;
		IP += 2;
		return;
	case 0xd6:		/* salc */
		AL = -CF;
		++IP;
		return;
	case 0xd7:		/* xlat */
		AL = (segpfx ? segpfx : DS.p)[(uint16_t) (AL + BX)];
		++IP;
		return;
	case 0xf4:		/* hlt */
		/* TODO: implement */
		++IP;
		return;
	case 0xf5:		/* cmc */
		CF ^= 1, ++IP;
		return;
	case 0xf8:		/* clc */
		CF = 0, ++IP;
		return;
	case 0xf9:		/* stc */
		CF = 1, ++IP;
		return;
	case 0xfa:		/* cli */
		IF = 0, ++IP;
		return;
	case 0xfb:		/* sti */
		IF = 1, ++IP;
		return;
	case 0xfc:		/* cld */
		DF = 0, ++IP;
		return;
	case 0xfd:		/* std */
		DF = 1, ++IP;
		return;
	case 0xa8:		/* test */
		setsfzfpf(AL & p[1]);
		OF = CF = 0, IP += 2;
		return;
	case 0xa9:
		setsfzfpf(AX & read16(p + 1));
		OF = CF = 0, IP += 3;
		return;
	case 0x0f:		/* hyper call */
		switch (p[1]) {
			time_t t;
#ifdef _WIN32
			static int skipcnt;
#endif
		case 0:	/* output char */
#ifdef _WIN32
			if (AL == 27)
				skipcnt = 6;
			if (skipcnt && skipcnt--)
				break;
#endif
			write(1, &AL, 1);
			break;
		case 1:	/* get time */
			time(&t);
			memcpy(&ES.p[BX], localtime(&t), 36);
			break;
		case 2:	/* read disk */
			if (fseek(files[DL], (256 * SI + BP) << 9, SEEK_SET) != -1)
				AL = fread(&ES.p[BX], 1, AX, files[DL]);
			else
				AL = 0;
			break;
		case 3:	/* write disk */
			if (fseek(files[DL], (256 * SI + BP) << 9, SEEK_SET) != -1)
				AL = fwrite(&ES.p[BX], 1, AX, files[DL]);
			else
				AL = 0;
			break;
		}
		IP += 2;
		return;
	}
	fprintf(stderr, "%04x:%04x %02x not implemented\n", CS.v, p - CS.p, b);
	exit(1);
}

int
main(int argc, char *argv[])
{
	setsr(&ES, 0);
	setsr(&SS, 0);
	setsr(&DS, 0);
	setsr(&CS, ROMBASE >> 4);
	IP = 0x100;

	for (int i = 0; i < 256; i++) {
		int n = 0;
		for (int j = 1; j < 256; j += j) {
			if (i & j)
				n++;
		}
		ptable[i] = (n & 1) == 0;
	}

	for (int i = 1; i <= 3 && i < argc; ++i)
		files[3 - i] = fopen(argv[i], "r+b");
	if (files[0])		/* CX:AX = HDD sectors */
		write32(&AX, fseek(files[0], 0, SEEK_END) >> 9);
	fread(&mem[ROMBASE + IP], 1, ROMBASE, files[2]);	/* read BIOS */

	uint16_t counter = 0;
	int kb = 0;
	for (;;) {
		if (!++counter) {
			kb = 1;
		}
		if (kb && IF) {
			intr(8);
#ifdef _WIN32
			if ((kb = kbhit())) {
				mem[0x4a6] = getch();
				intr(7);
			}
#else
			if ((kb = read(0, &mem[0x4a6], 1)))
				intr(7);
#endif
		}
		if (CS.v == 0 && IP == 0)
			break;
		step(0, NULL);
	}
}
