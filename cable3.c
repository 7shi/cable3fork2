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

#define POKE(dst,opr,src) (oldv=oprsz?*(uint16_t*)&dst:dst,newv=oprsz?*(uint16_t*)&dst opr(srcv=*(uint16_t*)&src):(dst opr(srcv=*(uint8_t*)&src)))

#define ROMBASE 0xf0000
uint8_t mem[0x200000 /* 2MB */ ], ioport[0x10000];
uint16_t r[13], IP;
uint8_t *const r8 = (uint8_t *) r;
uint8_t CF, PF, AF, ZF, SF, TF, IF, DF, OF;

uint8_t ptable[256];
extern uint8_t table14[], table18[], table19[], table20[], table51[];

uint8_t optype, oprsz;
int hassegpfx, segpfx;

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

#define ES r[8]
#define CS r[9]
#define SS r[10]
#define DS r[11]

void
push(uint16_t src)
{
	oprsz = 4;
	*(uint16_t *) &mem[16 * SS + (SP -= 2)] = src;
}

uint16_t
pop(void)
{
	oprsz = 4;
	uint16_t ret = *(uint16_t *) &mem[16 * SS + SP];
	SP += 2;
	return ret;
}

uint8_t *
regmap(int reg)
{
	return &r8[oprsz ? 2 * reg : (2 * reg + reg / 4) & 7];
}

void
setafof(uint16_t srcv, uint16_t oldv, uint16_t newv)
{
	AF = !!((srcv ^= oldv ^ newv) & 16);
	OF = (newv - oldv && 1 & (CF ^ srcv >> (8 * (oprsz + 1) - 1)));
}

void
setsfzfpf(uint16_t newv)
{
	SF = (1 & (oprsz ? *(int16_t *) &newv : newv) >> (8 * (oprsz + 1) - 1));
	ZF = !newv;
	PF = ptable[(uint8_t) newv];
}

uint16_t
getflags(void)
{
	return 0xf002 | CF | (PF << 2) | (AF << 4) | (ZF << 6) | (SF << 7) | (TF << 8) | (IF << 9) | (DF << 10) | (OF << 11);
}

void
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

void
intr(int n)
{
	push(getflags());
	push(CS);
	push(IP);
	CS = *(uint16_t *) &mem[4 * n + 2];
	IP = *(uint16_t *) &mem[4 * n];
	IF = 0;
}

uint8_t *
modrm(int *oprlen, int mode, int rm, int16_t disp)
{
	if (mode == 3) {
		*oprlen = 0;
		return regmap(rm);
	}
	*oprlen = mode;
	uint16_t addr, *seg;
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
	if (hassegpfx)
		seg = &r[segpfx];
	if (mode)
		addr += disp;
	return &mem[16 * *seg + addr];
}

void
getoprs(int dir, int reg, uint8_t *addr, uint8_t **opr1, uint8_t **opr2)
{
	if (!dir)
		*opr1 = addr, *opr2 = regmap(reg);
	else
		*opr1 = regmap(reg), *opr2 = addr;
}

int
main(int argc, char *argv[])
{
	CS = ROMBASE >> 4, IP = 0x100;

	for (int i = 0; i < 256; i++) {
		int n = 0;
		for (int j = 1; j < 256; j += j) {
			if (i & j)
				n++;
		}
		ptable[i] = (n & 1) == 0;
	}

	FILE *files[] = {NULL /* HD */ , NULL /* FD */ , NULL /* BIOS */ };
	for (int i = 1; i <= 3 && i < argc; ++i)
		files[3 - i] = fopen(argv[i], "r+b");
	if (files[0])		/* CX:AX = HDD sectors */
		*(uint32_t *) r = fseek(files[0], 0, SEEK_END) >> 9;
	fread(&mem[ROMBASE + IP], 1, ROMBASE, files[2]);	/* read BIOS */

	int hasrep = 0, rep = 0, kb = 0;
	uint16_t counter = 0;

	for (;;) {
		if (!++counter) {
			kb = 1;
			if (ioport[0]) {
				fprintf(stderr, "HGC not supported\n");
				return 1;
			}
		}
		if (!hassegpfx && !rep && kb && IF) {
			intr(8);
#ifdef _WIN32
			if (kb = kbhit()) {
				mem[0x4a6] = getch();
				intr(7);
			}
#else
			if (kb = read(0, &mem[0x4a6], 1))
				intr(7);
#endif
		}
		if (CS == 0 && IP == 0)
			break;
		ioport[32] = 0;
		--ioport[64];
		uint8_t *ipptr = &mem[16 * CS + IP];
		oprsz = ipptr[0] & 1;
		int o0 = ipptr[0] & 7, dir = ipptr[0] / 2 & 1;
		uint32_t w1 = *(int16_t *) &ipptr[1];
		uint32_t w2 = *(int16_t *) &ipptr[2];
		uint32_t w3 = *(int16_t *) &ipptr[3];
		int o1a = ipptr[1] & 7, o1b = ipptr[1] / 8 & 7;
		int mode = ipptr[1] >> 6;
		int16_t disp = mode != 1 ? w2 : (int8_t) w2;
		uint32_t opr = w3;
		if (!(mode == 0 && o1a == 6) && mode != 2) {
			if (mode != 1)
				opr = disp;
		} else
			opr = *(int16_t *) &ipptr[4];
		if (hassegpfx)
			hassegpfx--;
		if (rep)
			rep--;
		int oprlen;
		uint8_t *addr, *opr1, *opr2;
		getoprs(dir, o1b, addr = modrm(&oprlen, mode, o1a, disp), &opr1, &opr2);
		optype = table51[ipptr[0]];
		uint8_t oprtype = table14[optype];
		switch (optype) {
			int tmp, tmp2;
			uint32_t utmp;
			uint16_t srcv, oldv, newv;
		case 0:	/* conditional jump, enter?, leave?, int1? */
			switch (ipptr[0]) {
			case 0x70:	/* jo */
				tmp = OF;
				break;
			case 0x71:	/* jno */
				tmp = !OF;
				break;
			case 0x72:	/* jb, jnae */
				tmp = CF;
				break;
			case 0x73:	/* jnb, jae */
				tmp = !CF;
				break;
			case 0x74:	/* je, jz */
				tmp = ZF;
				break;
			case 0x75:	/* jne, jnz */
				tmp = !ZF;
				break;
			case 0x76:	/* jbe, jna */
				tmp = CF || ZF;
				break;
			case 0x77:	/* jnbe, ja */
				tmp = !CF && !ZF;
				break;
			case 0x78:	/* js */
				tmp = SF;
				break;
			case 0x79:	/* jns */
				tmp = !SF;
				break;
			case 0x7a:	/* jp */
				tmp = PF;
				break;
			case 0x7b:	/* jnp */
				tmp = !PF;
				break;
			case 0x7c:	/* jl, jnge */
				tmp = SF != OF;
				break;
			case 0x7d:	/* jnl, jge */
				tmp = SF == OF;
				break;
			case 0x7e:	/* jle, jng */
				tmp = ZF || SF != OF;
				break;
			case 0x7f:	/* jnle, jg */
				tmp = !ZF && SF == OF;
				break;
			default:
				tmp = 0;
				break;
			}
			IP += 2;
			if (tmp)
				IP += (int8_t) ipptr[1];
			break;
		case 1:	/* mov */
			oprsz = ipptr[0] & 8;
			addr = regmap(o0);
			POKE(*addr, =, w1);
			IP += oprsz ? 3 : 2;
			break;
		case 2:	/* inc */
		case 3:	/* dec */
			/* oprtype = 0, 1 */
			oprsz = 2, o1b = oprtype;
			opr1 = addr = modrm(&oprlen, mode, o1a, disp), opr2 = regmap(o0);
		case 6:	/* inc, dec, call, callf, jmp, jmpf, push */
			if (o1b < 2) {	/* inc, dec */
				POKE(*opr2, +=1 - 2 * o1b +, r[12]);
				srcv = 1;
				setafof(srcv, oldv, newv);
				setsfzfpf(newv);
				OF = (oldv + 1 - o1b == 1 << (8 * (oprsz + 1) - 1));
				if (optype == 6)
					IP += 2 + oprlen;
				else
					++IP;
			} else if (o1b != 6) {
				IP += 2 + oprlen;
				if (o1b == 3)	/* callf */
					push(CS);
				if (o1b & 2)	/* call, callf */
					push(IP);
				if (o1b & 1)	/* jmpf */
					CS = *(uint16_t *) &opr2[2];
				IP = *(uint16_t *) opr2;
			} else {
				push(*(uint16_t *) addr);
				IP += 2 + oprlen;
			}
			break;
		case 4:	/* push */
			push(r[o0]);
			++IP;
			break;
		case 5:	/* pop */
			r[o0] = pop();
			++IP;
			break;
		case 7:	/* test, not, neg, mul, imul, div, idiv */
			opr1 = opr2;
			switch (o1b) {
			case 0:/* test */
				IP -= ~oprsz;
				POKE(*opr1, &, opr);
				setsfzfpf(newv);
				OF = CF = 0;
				break;
			case 2:/* not */
				POKE(*opr1, = ~, *opr2);
				break;
			case 3:/* neg */
				POKE(*opr1, = -, *opr2);
				CF = newv > (oldv = 0);
				setafof(srcv, oldv, newv);
				setsfzfpf(newv);
				break;
			case 4:/* mul */
				if (oprsz) {
					DX = (AX = utmp = *(uint16_t *) addr * AX) >> 16;
					OF = CF = (utmp >> 16) != 0;
				} else {
					AX = utmp = *addr * AL;
					OF = CF = (utmp >> 8) != 0;
				}
				setsfzfpf(newv);
				break;
			case 5:/* imul */
				if (oprsz) {
					DX = (AX = tmp = *(int16_t *) addr * (int16_t) AX) >> 16;
					OF = CF = (tmp >> 16) != 0;
				} else {
					AX = tmp = *(int8_t *) addr *(int8_t) AL;
					OF = CF = (tmp >> 8) != 0;
				}
				setsfzfpf(newv);
				break;
			case 6:/* div */
				if (oprsz) {
					if (tmp2 = *(uint16_t *) addr) {
						utmp = (uint32_t) (tmp = (DX << 16) + AX) / tmp2;
						if (!(utmp - (uint16_t) utmp))
							DX = tmp - tmp2 * (AX = utmp);
						else
							intr(0);
					}
				} else {
					if (tmp2 = *addr) {
						utmp = (uint16_t) (tmp = (AH << 16) + AX) / tmp2;
						if (!(utmp - (uint8_t) utmp))
							AH = tmp - tmp2 * (AL = utmp);
						else
							intr(0);
					}
				}
				break;
			case 7:/* idiv */
				if (oprsz) {
					if (tmp2 = *(int16_t *) addr) {
						utmp = (int) (tmp = (DX << 16) + AX) / tmp2;
						if (!(utmp - (int16_t) utmp))
							DX = tmp - tmp2 * (AX = utmp);
						else
							intr(0);
					}
				} else {
					if (tmp2 = *(int8_t *) addr) {
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
			break;
		case 8:	/* add */
		case 9:	/* or */
		case 10:	/* adc */
		case 11:	/* sbb */
		case 12:	/* and */
		case 13:	/* sub */
		case 14:	/* xor */
		case 15:	/* cmp */
			/* oprtype = 0, 1, 2, 3, 4, 5, 6, 7 */
			addr = r8, opr = w1, mode = 3, o1b = oprtype, IP--, oprlen = 0;
		case 16:	/* add, or, adc, sbb, and, sub, xor, cmp */
			opr1 = addr, opr2 = (uint8_t *) &utmp;
			utmp = (dir |= !oprsz) ? (int8_t) opr : opr;
			IP -= ~!dir, optype = 17 + (oprtype = o1b);
		case 17:	/* add */
		case 18:	/* or */
		case 19:	/* adc */
		case 20:	/* sbb */
		case 21:	/* and */
		case 22:	/* sub */
		case 23:	/* xor */
		case 24:	/* cmp */
		case 25:	/* mov */
			/* oprtype = 0, 1, 2, 3, 4, 5, 6, 7, 8 */
			switch (oprtype) {
			case 0:/* add */
				POKE(*opr1, +=, *opr2);
				CF = newv < oldv;
				setafof(srcv, oldv, newv);
				break;
			case 1:/* or */
				POKE(*opr1, |=, *opr2);
				OF = CF = 0;
				break;
			case 2:/* adc */
				POKE(*opr1, +=CF +, *opr2);
				CF = !!(CF & newv == oldv | +newv < +(int) oldv);
				setafof(srcv, oldv, newv);
				break;
			case 3:/* sbb */
				POKE(*opr1, -=CF +, *opr2);
				CF = !!(CF & newv == oldv | -newv < -(int) oldv);
				setafof(srcv, oldv, newv);
				break;
			case 4:/* and */
				POKE(*opr1, &=, *opr2);
				OF = CF = 0;
				break;
			case 5:/* sub */
				POKE(*opr1, -=, *opr2);
				CF = newv > oldv;
				setafof(srcv, oldv, newv);
				break;
			case 6:/* xor */
				POKE(*opr1, ^=, *opr2);
				OF = CF = 0;
				break;
			case 7:/* cmp */
				POKE(*opr1, -, *opr2);
				CF = newv > oldv;
				setafof(srcv, oldv, newv);
				break;
			case 8:/* mov */
				POKE(*opr1, =, *opr2);
				break;
			}
			if (oprtype != 8)
				setsfzfpf(newv);
			IP += 2 + oprlen;
			break;
		case 26:	/* mov, lea, pop */
			if (!oprsz) {
				oprsz = o1b + 8;
				getoprs(dir, oprsz, modrm(&oprlen, mode, o1a, disp), &opr1, &opr2);
				POKE(*opr1, =, *opr2);
			} else if (!dir) {
				hassegpfx = 1, segpfx = 12;
				*(uint16_t *) opr2 = modrm(&oprlen, mode, o1a, disp) - mem;
			} else
				*(uint16_t *) addr = pop();
			IP += 2 + oprlen;
			break;
		case 27:	/* mov */
			getoprs(dir, 0, modrm(&oprlen, 0, 6, w1), &opr1, &opr2);
			POKE(*opr2, =, *opr1);
			IP += 3;
			break;
		case 28:	/* rol, ror, rcl, rcr, shl, sal, shr, sar */
		case 94:	/* rcl */
			utmp = (1 & (oprsz ? *(int16_t *) addr : *addr) >> (8 * (oprsz + 1) - 1));
			/* oprtype = 0, 1 */
			if (oprtype) {
				++IP;
				tmp = (int8_t) disp;
			} else if (dir)
				tmp = 31 & CL;
			else
				tmp = 1;
			if (tmp) {
				if (o1b < 4) {
					tmp %= o1b / 2 + 8 * (oprsz + 1);
					POKE(utmp, =, *addr);
				}
				if (o1b & 1)
					POKE(*addr, >>=, tmp);
				else
					POKE(*addr, <<=, tmp);
				if (o1b >= 5)
					CF = oldv >> tmp - 1 & 1;
			}
			switch (o1b) {
			case 0:/* rol */
				POKE(*addr, +=, utmp >> 8 * (oprsz + 1) - tmp);
				OF = (1 & (oprsz ? *(int16_t *) &newv : newv) >> (8 * (oprsz + 1) - 1)) ^ (CF = newv & 1);
				break;
			case 1:/* ror */
				utmp &= (1 << tmp) - 1;
				POKE(*addr, +=, utmp << (8 * (oprsz + 1) - tmp));
				CF = !!(1 & (oprsz ? *(int16_t *) &newv : newv) >> (8 * (oprsz + 1) - 1));
				OF = (1 & (oprsz ? *(int16_t *) &newv * 2 : newv * 2) >> (8 * (oprsz + 1) - 1)) ^ CF;
				break;
			case 2:/* rcl */
				POKE(*addr, +=(CF << tmp - 1) +, utmp >> (1 + 8 * (oprsz + 1) - tmp));
				CF = !!(utmp & 1 << (8 * (oprsz + 1) - tmp));
				OF = (1 & (oprsz ? *(int16_t *) &newv : newv) >> (8 * (oprsz + 1) - 1)) ^ CF;
				break;
			case 3:/* rcr */
				POKE(*addr, +=(CF << (8 * (oprsz + 1) - tmp)) +, utmp << (1 + 8 * (oprsz + 1) - tmp));
				CF = !!(utmp & 1 << tmp - 1);
				OF = (1 & (oprsz ? *(int16_t *) &newv : newv) >> (8 * (oprsz + 1) - 1)) ^ (1 & (oprsz ? *(int16_t *) &newv * 2 : newv * 2) >> (8 * (oprsz + 1) - 1));
				break;
			case 4:/* shl */
				CF = !!(1 & (oprsz ? *(int16_t *) &oldv << tmp - 1 : oldv << tmp - 1) >> (8 * (oprsz + 1) - 1));
				OF = (1 & (oprsz ? *(int16_t *) &newv : newv) >> (8 * (oprsz + 1) - 1)) ^ CF;
				break;
			case 5:/* shr */
				OF = (1 & (oprsz ? *(int16_t *) &oldv : oldv) >> (8 * (oprsz + 1) - 1));
				break;
			case 7:/* sar */
				if (tmp >= 8 * (oprsz + 1))
					CF = !!utmp;
				OF = 0;
				POKE(*addr, +=, utmp *= ~(((1 << (8 * (oprsz + 1))) - 1) >> tmp));
				break;
			}
			if (tmp && o1b > 3) {
				setsfzfpf(newv);
				IP += 2 + oprlen;
			} else
				IP += 2 + oprlen + (optype == 94);
			break;
		case 29:	/* loopnz, loopne, loopz, loope, loop, jcxz */
			tmp = !!--CX;
			switch (o0) {
			case 0:/* loopnz, loopne */
				tmp &= !ZF;
				break;
			case 1:/* loopz, loope */
				tmp &= ZF;
				break;
			case 3:/* jcxz */
				tmp = !++CX;
				break;
			}
			IP += 2;
			if (tmp)
				IP += (int8_t) ipptr[1];
			break;
		case 30:	/* call, jmp, jmpf */
			IP += 3 - dir;
			if (!oprsz) {
				if (dir)
					CS = w3, IP = 0;
				else
					push(IP);
			}
			IP += dir * oprsz ? (int8_t) w1 : w1;
			break;
		case 31:	/* test */
			POKE(*opr2, &, *opr1);
			setsfzfpf(newv);
			OF = CF = 0;
			IP += 2 + oprlen;
			break;
		case 32:	/* xchg */
			oprsz = 7, opr1 = r8, opr2 = regmap(o0);
		case 48:	/* xchg */
			if (opr1 != opr2) {
				POKE(*opr1, ^=, *opr2);
				POKE(*opr2, ^=, *opr1);
				POKE(*opr1, ^=, *opr2);
			}
			if (optype == 32)
				++IP;
			else
				IP += 2 + oprlen;
			break;
		case 33:	/* movsb, movsw */
		case 36:	/* stosb, stosw */
		case 37:	/* lodsb,lodsw */
			if (!rep || CX) {
				/* oprtype = 0, 1, 2 */
				opr1 = oprtype < 2 ? &mem[16 * ES + DI] : r8;
				opr2 = oprtype & 1 ? r8 : &mem[16 * (hassegpfx ? r[segpfx] : DS) + SI];
				POKE(*opr1, =, *opr2);
				tmp = ~(-2 * DF) * ~oprsz;
				if (!(oprtype & 1))
					SI += tmp;
				if (!(oprtype & 2)) {
					DI += tmp;
					if (rep && --CX) {
						rep++;
						if (hassegpfx)
							hassegpfx++;
						IP--;
					};
				}
			}
			++IP;
			break;
		case 34:	/* cmpsb, cmpsw */
		case 38:	/* scasb, scasw */
			if (!rep || CX) {
				/* oprtype = 0, 1 */
				opr1 = oprtype ? r8 : &mem[16 * (hassegpfx ? r[segpfx] : DS) + SI];
				opr2 = &mem[16 * ES + DI];
				POKE(*opr1, -, *opr2);
				optype = 92;
				ZF = !newv;
				CF = newv > oldv;
				tmp = ~(-2 * DF) * ~oprsz;
				if (!oprtype)
					SI += tmp;
				DI += tmp;
				if (rep && --CX && !newv == hasrep) {
					rep++;
					if (hassegpfx)
						hassegpfx++;
					IP--;
				}
				setafof(srcv, oldv, newv);
				setsfzfpf(newv);
			}
			++IP;
			break;
		case 39:	/* ret */
		case 41:	/* retf */
		case 78:	/* iret */
			dir = oprsz;
			IP = pop();
			/* oprtype = 0, 1, 2 */
			if (oprtype)
				CS = pop();
			if (oprtype & 2)
				setflags(pop());
			else if (!dir)
				SP += w1;
			break;
		case 40:	/* mov */
			POKE(*opr2, =, opr);
			IP += 3 + oprlen + oprsz;
			break;
		case 42:	/* in */
			++IP;
		case 44:	/* in */
			ioport[0x3da] ^= 9;
			/* oprtype = 0, 1 */
			POKE(AL, =, ioport[oprtype ? DX : (int8_t) w1]);
			++IP;
			break;
		case 43:	/* out */
			++IP;
		case 45:	/* out */
			/* oprtype = 0, 1 */
			POKE(ioport[oprtype ? DX : (int8_t) w1], =, AL);
			++IP;
			break;
		case 46:	/* repnz, repz */
			rep = 2, hasrep = oprsz;
			if (hassegpfx)
				hassegpfx++;
			++IP;
			break;
		case 49:	/* push */
		case 51:	/* push */
		case 52:	/* push */
		case 54:	/* push */
			/* oprtype = 8: ES, 9: CS, 10: SS, 11: DS */
			push(r[oprtype]);
			++IP;
			break;
		case 50:	/* pop */
		case 53:	/* pop */
		case 55:	/* pop */
			/* oprtype = 8: ES, 10: SS, 11: DS */
			r[oprtype] = pop();
			++IP;
			break;
		case 56:	/* es: */
		case 58:	/* cs: */
		case 60:	/* ss: */
		case 62:	/* ds: */
			/* oprtype = 8: ES, 9: CS, 10: SS, 11: DS */
			hassegpfx = 2, segpfx = oprtype;
			if (rep)
				rep++;
			++IP;
			break;
		case 57:	/* daa */
		case 59:	/* das */
		case 61:	/* aaa */
		case 63:	/* aas */
			fprintf(stderr, "not implemented: daa/das/aaa/aas\n");
			exit(1);
			/* setsfzfpf(newv); ++IP; */
			break;
		case 65:	/* cbw */
			AH = -(1 & (oprsz ? (int16_t) AX : AL) >> (8 * (oprsz + 1) - 1));
			++IP;
			break;
		case 66:	/* cwd */
			DX = -(1 & (oprsz ? (int16_t) AX : AX) >> (8 * (oprsz + 1) - 1));
			++IP;
			break;
		case 67:	/* callf */
			push(CS);
			push(IP + 5);
			CS = w3, IP = w1;
			break;
		case 69:	/* pushf */
			push(getflags());
			++IP;
			break;
		case 70:	/* popf */
			setflags(pop());
			++IP;
			break;
		case 71:	/* sahf */
			setflags((getflags() & ~255) + AH);
			++IP;
			break;
		case 72:	/* lahf */
			AH = getflags();
			++IP;
			break;
		case 73:	/* les */
		case 74:	/* lds */
			oprsz = 1;
			opr1 = regmap(o1b), opr2 = modrm(&oprlen, mode, o1a, disp);
			POKE(*opr1, =, *opr2);
			/* oprtype = 16: ES, 22: DS */
			POKE(r8[oprtype], =, opr2[2]);
			IP += 2 + oprlen;
			break;
		case 75:	/* int3 */
			++IP;
			intr(3);
			break;
		case 76:	/* int */
			IP += 2;
			intr(w1 & 255);
			break;
		case 77:	/* into */
			++IP;
			if (OF)
				intr(4);
			break;
		case 79:	/* aam */
			if (w1 &= 255) {
				AH = AL / w1;
				newv = AL %= w1;
			} else
				intr(0);
			setsfzfpf(newv);
			OF = CF = 0;
			IP += 2;
			break;
		case 80:	/* aad */
			oprsz = 0, AX = newv = AL + w1 * AH;
			setsfzfpf(newv);
			OF = CF = 0;
			IP += 2;
			break;
		case 81:	/* salc */
			AL = -CF;
			++IP;
			break;
		case 82:	/* xlat */
			AL = mem[16 * (hassegpfx ? r[segpfx] : DS) + (uint16_t) (AL + BX)];
			++IP;
			break;
		case 85:	/* cmc */
			CF ^= 1;
			++IP;
			break;
		case 86:	/* clc */
			CF = 0;
			++IP;
			break;
		case 87:	/* stc */
			CF = 1;
			++IP;
			break;
		case 88:	/* cli */
			IF = 0;
			++IP;
			break;
		case 89:	/* sti */
			IF = 1;
			++IP;
			break;
		case 90:	/* cld */
			DF = 0;
			++IP;
			break;
		case 91:	/* std */
			DF = 1;
			++IP;
			break;
		case 35:	/* test */
			POKE(AL, &, w1);
			setsfzfpf(newv);
			OF = CF = 0;
			IP += 2 + oprsz;
			break;
		case 64:	/* hyper call */
			switch ((uint8_t) w1) {
				time_t t;
#ifdef _WIN32
				static int skipcnt;
#endif
			case 0:/* output char */
#ifdef _WIN32
				if (AL == 27)
					skipcnt = 6;
				if (skipcnt && skipcnt--)
					break;
#endif
				write(1, &AL, 1);
				break;
			case 1:/* get time */
				time(&t);
				memcpy(&mem[16 * ES + BX], localtime(&t), 36);
				break;
			case 2:/* read disk */
				if (fseek(files[DL], (256 * SI + BP) << 9, SEEK_SET) != -1)
					AL = fread(&mem[16 * ES + BX], 1, AX, files[DL]);
				else
					AL = 0;
				break;
			case 3:/* write disk */
				if (fseek(files[DL], (256 * SI + BP) << 9, SEEK_SET) != -1)
					AL = fwrite(&mem[16 * ES + BX], 1, AX, files[DL]);
				else
					AL = 0;
				break;
			}
			IP += 2;
			break;
		}
	}
}
