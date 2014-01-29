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
uint8_t *const r8 = &mem[ROMBASE];
uint16_t *const r = (uint16_t *) &mem[ROMBASE];
uint8_t CF, PF, AF, ZF, SF, TF, IF, DF, OF;

uint8_t ptable[256];
extern uint8_t table14[], table15[], table18[], table19[], table20[], table25[];
extern uint8_t table51[];

uint8_t optype, oprsz;
uint16_t ip, srcv, oldv, newv;
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
setafof(void)
{
	AF = !!((srcv ^= oldv ^ newv) & 16);
	OF = (newv - oldv && 1 & (CF ^ srcv >> (8 * (oprsz + 1) - 1)));
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
	optype = 76;
	push(getflags());
	push(CS);
	push(ip);
	POKE(CS, =, mem[4 * n + 2]);
	POKE(ip, =, mem[4 * n]);
	IF = 0;
}

uint8_t *
modrm(int mode, int rm, int16_t disp)
{
	if (mode == 3)
		return regmap(rm);
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
		if (mode == 0)
			seg = &DS, addr = disp;
		else
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
	CS = ROMBASE >> 4, ip = 0x100;

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
	fread(&mem[ROMBASE + ip], 1, ROMBASE, files[2]);	/* read BIOS */

	int hasrep = 0, rep = 0, kb = 0;
	uint16_t counter = 0;

	for (;;) {
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
		if (CS == 0 && ip == 0)
			break;
		ioport[32] = 0;
		--ioport[64];
		uint8_t *ipptr = &mem[16 * CS + ip];
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
		uint8_t *addr = modrm(mode, o1a, disp), *opr1, *opr2;
		getoprs(dir, o1b, addr, &opr1, &opr2);
		optype = table51[ipptr[0]];
		uint8_t oprtype = table14[optype];
		switch (optype) {
			int tmp, tmp2;
			uint32_t utmp;
		case 0:	/* conditional jump, enter?, leave?, int1? */
			switch (ipptr[0]) {
			case 0x70:	/* jo */
				if (OF)
					ip += (int8_t) w1;
				break;
			case 0x71:	/* jno */
				if (!OF)
					ip += (int8_t) w1;
				break;
			case 0x72:	/* jb, jnae */
				if (CF)
					ip += (int8_t) w1;
				break;
			case 0x73:	/* jnb, jae */
				if (!CF)
					ip += (int8_t) w1;
				break;
			case 0x74:	/* je, jz */
				if (ZF)
					ip += (int8_t) w1;
				break;
			case 0x75:	/* jne, jnz */
				if (!ZF)
					ip += (int8_t) w1;
				break;
			case 0x76:	/* jbe, jna */
				if (CF || ZF)
					ip += (int8_t) w1;
				break;
			case 0x77:	/* jnbe, ja */
				if (!CF && !ZF)
					ip += (int8_t) w1;
				break;
			case 0x78:	/* js */
				if (SF)
					ip += (int8_t) w1;
				break;
			case 0x79:	/* jns */
				if (!SF)
					ip += (int8_t) w1;
				break;
			case 0x7a:	/* jp */
				if (PF)
					ip += (int8_t) w1;
				break;
			case 0x7b:	/* jnp */
				if (!PF)
					ip += (int8_t) w1;
				break;
			case 0x7c:	/* jl, jnge */
				if (SF != OF)
					ip += (int8_t) w1;
				break;
			case 0x7d:	/* jnl, jge */
				if (SF == OF)
					ip += (int8_t) w1;
				break;
			case 0x7e:	/* jle, jng */
				if (ZF || SF != OF)
					ip += (int8_t) w1;
				break;
			case 0x7f:	/* jnle, jg */
				if (!ZF && SF == OF)
					ip += (int8_t) w1;
				break;
			}
			break;
		case 1:	/* mov */
			oprsz = ipptr[0] & 8;
			tmp = regmap(o0) - mem;
			POKE(mem[tmp], =, w1);
			break;
		case 2:	/* inc */
		case 3:	/* dec */
			/* oprtype = 0, 1 */
			oprsz = 2, o1b = oprtype;
			opr1 = addr = modrm(mode, o1a, disp), opr2 = regmap(o0);
		case 6:	/* inc, dec, call, callf, jmp, jmpf, push */
			if (o1b < 2) {
				POKE(*opr2, +=1 - 2 * o1b +, r[12]);
				srcv = 1;
				setafof();
				OF = (oldv + 1 - o1b == 1 << (8 * (oprsz + 1) - 1));
				optype = optype & 4 ? 19 : 57;
			} else if (o1b != 6) {
				ip += (mode % 3 + 2 * !(!mode * o1a - 6)) + 2;
				if (o1b == 3)
					push(CS);
				if (o1b & 2)
					push(ip);
				if (o1b & 1)
					POKE(CS, =, opr2[2]);
				POKE(ip, =, *opr2);
				optype = 67;
			} else
				push(*(uint16_t *) addr);
			break;
		case 4:	/* push */
			push(r[o0]);
			break;
		case 5:	/* pop */
			r[o0] = pop();
			break;
		case 7:	/* test, not, neg, mul, imul, div, idiv */
			opr1 = opr2;
			switch (o1b) {
			case 0:/* test */
				optype = 21, ip -= ~oprsz;
				POKE(*opr1, &, opr);
				OF = CF = 0;
				break;
			case 2:/* not */
				POKE(*opr1, = ~, *opr2);
				break;
			case 3:/* neg */
				POKE(*opr1, = -, *opr2);
				oldv = 0, optype = 22;
				CF = newv > oldv;
				setafof();
				break;
			case 4:/* mul */
				optype = 19;
				if (oprsz) {
					DX = (AX = newv = *(uint16_t *) addr * AX) >> 16;
					OF = CF = !!(newv - (uint16_t) newv);
				} else {
					AX = newv = *addr * AL;
					OF = CF = !!(newv - (uint8_t) newv);
				}
				break;
			case 5:/* imul */
				optype = 19;
				if (oprsz) {
					DX = (AX = newv = *(int16_t *) addr * (int16_t) AX) >> 16;
					OF = CF = !!(newv - (int16_t) newv);
				} else {
					AX = newv = *(int8_t *) addr *(int8_t) AL;
					OF = CF = !!(newv - (int8_t) newv);
				}
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
			addr = r8, opr = w1, mode = 3, o1b = oprtype, ip--;
		case 16:	/* add, or, adc, sbb, and, sub, xor, cmp */
			opr1 = addr, opr2 = (uint8_t *) &r[13];
			r[13] = (dir |= !oprsz) ? (int8_t) opr : opr;
			ip -= ~!dir, optype = 17 + (oprtype = o1b);
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
				setafof();
				break;
			case 1:/* or */
				POKE(*opr1, |=, *opr2);
				OF = CF = 0;
				break;
			case 2:/* adc */
				POKE(*opr1, +=CF +, *opr2);
				CF = !!(CF & newv == oldv | +newv < +(int) oldv);
				setafof();
				break;
			case 3:/* sbb */
				POKE(*opr1, -=CF +, *opr2);
				CF = !!(CF & newv == oldv | -newv < -(int) oldv);
				setafof();
				break;
			case 4:/* and */
				POKE(*opr1, &=, *opr2);
				OF = CF = 0;
				break;
			case 5:/* sub */
				POKE(*opr1, -=, *opr2);
				CF = newv > oldv;
				setafof();
				break;
			case 6:/* xor */
				POKE(*opr1, ^=, *opr2);
				OF = CF = 0;
				break;
			case 7:/* cmp */
				POKE(*opr1, -, *opr2);
				CF = newv > oldv;
				setafof();
				break;
			case 8:/* mov */
				POKE(*opr1, =, *opr2);
				break;
			}
			break;
		case 26:	/* mov, lea, pop */
			if (!oprsz) {
				oprsz = o1b + 8;
				getoprs(dir, oprsz, modrm(mode, o1a, disp), &opr1, &opr2);
				POKE(*opr1, =, *opr2);
			} else if (!dir) {
				hassegpfx = 1, segpfx = 12, tmp = modrm(mode, o1a, disp) - mem;
				POKE(*opr2, =, tmp);
			} else
				*(uint16_t *) addr = pop();
			break;
		case 27:	/* mov */
			getoprs(dir, 0, modrm(0, 6, w1), &opr1, &opr2);
			POKE(*opr2, =, *opr1);
			break;
		case 28:	/* rol, ror, rcl, rcr, shl, sal, shr, sar */
		case 94:	/* rcl */
			utmp = (1 & (oprsz ? *(int16_t *) addr : *addr) >> (8 * (oprsz + 1) - 1));
			/* oprtype = 0, 1 */
			if (tmp = oprtype ? ++ip, (int8_t) disp : dir ? 31 & CL : 1) {
				if (o1b < 4) {
					tmp %= o1b / 2 + 8 * (oprsz + 1);
					POKE(utmp, =, *addr);
				}
				if (o1b & 1)
					POKE(*addr, >>=, tmp);
				else
					POKE(*addr, <<=, tmp);
				if (o1b > 3)
					optype = 19;
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
			ip += tmp * (int8_t) w1;
			break;
		case 30:	/* call, jmp, jmpf */
			ip += 3 - dir;
			if (!oprsz) {
				if (dir)
					CS = w3, ip = 0;
				else
					push(ip);
			}
			ip += dir * oprsz ? (int8_t) w1 : w1;
			break;
		case 31:	/* test */
			POKE(*opr2, &, *opr1);
			OF = CF = 0;
			break;
		case 32:	/* xchg */
			oprsz = 7, opr1 = r8, opr2 = regmap(o0);
		case 48:	/* xchg */
			if (opr1 != opr2) {
				POKE(*opr1, ^=, *opr2);
				POKE(*opr2, ^=, *opr1);
				POKE(*opr1, ^=, *opr2);
			}
			break;
		case 33:	/* movsb, movsw */
		case 36:	/* stosb, stosw */
		case 37:	/* lodsb,lodsw */
			if (!rep || CX) {
				/* oprtype = 0, 1, 2 */
				opr1 = oprtype < 2 ? &mem[16 * ES + DI] : r8;
				opr2 = oprtype & 1 ? r8 : &mem[16 * r[hassegpfx ? segpfx : 11] + SI];
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
						ip--;
					};
				}
			}
			break;
		case 34:	/* cmpsb, cmpsw */
		case 38:	/* scasb, scasw */
			if (!rep || CX) {
				/* oprtype = 0, 1 */
				opr1 = oprtype ? r8 : &mem[16 * r[hassegpfx ? segpfx : 11] + SI];
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
					ip--;
				}
				setafof();
			}
			break;
		case 39:	/* ret */
		case 41:	/* retf */
		case 78:	/* iret */
			dir = oprsz;
			ip = pop();
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
			break;
		case 42:	/* in */
		case 44:	/* in */
			ioport[0x3da] ^= 9;
			/* oprtype = 0, 1 */
			POKE(AL, =, ioport[oprtype ? DX : (int8_t) w1]);
			break;
		case 43:	/* out */
		case 45:	/* out */
			/* oprtype = 0, 1 */
			POKE(ioport[oprtype ? DX : (int8_t) w1], =, AL);
			break;
		case 46:	/* repnz, repz */
			rep = 2, hasrep = oprsz;
			if (hassegpfx)
				hassegpfx++;
			break;
		case 49:	/* push */
		case 51:	/* push */
		case 52:	/* push */
		case 54:	/* push */
			/* oprtype = 8: ES, 9: CS, 10: SS, 11: DS */
			push(r[oprtype]);
			break;
		case 50:	/* pop */
		case 53:	/* pop */
		case 55:	/* pop */
			/* oprtype = 8: ES, 10: SS, 11: DS */
			r[oprtype] = pop();
			break;
		case 56:	/* es: */
		case 58:	/* cs: */
		case 60:	/* ss: */
		case 62:	/* ds: */
			/* oprtype = 8: ES, 9: CS, 10: SS, 11: DS */
			hassegpfx = 2, segpfx = oprtype;
			if (rep)
				rep++;
			break;
		case 57:	/* daa */
		case 59:	/* das */
		case 61:	/* aaa */
		case 63:	/* aas */
			fprintf(stderr, "not implemented: daa/das/aaa/aas\n");
			exit(1);
			break;
		case 65:	/* cbw */
			AH = -(1 & (oprsz ? *(int16_t *) r8 : AL) >> (8 * (oprsz + 1) - 1));
			break;
		case 66:	/* cwd */
			DX = -(1 & (oprsz ? *(int16_t *) r : AX) >> (8 * (oprsz + 1) - 1));
			break;
		case 67:	/* callf */
			push(CS);
			push(ip + 5);
			CS = w3, ip = w1;
			break;
		case 69:	/* pushf */
			push(getflags());
			break;
		case 70:	/* popf */
			setflags(pop());
			break;
		case 71:	/* sahf */
			setflags((getflags() & ~255) + AH);
			break;
		case 72:	/* lahf */
			AH = getflags();
			break;
		case 73:	/* les */
		case 74:	/* lds */
			oprsz = 1;
			opr1 = regmap(o1b), opr2 = modrm(mode, o1a, disp);
			POKE(*opr1, =, *opr2);
			/* oprtype = 16, 22 */
			POKE(r8[oprtype], =, opr2[2]);
			break;
		case 75:	/* int3 */
			++ip;
			intr(3);
			break;
		case 76:	/* int */
			ip += 2;
			intr(w1 & 255);
			break;
		case 77:	/* into */
			++ip;
			if (OF)
				intr(4);
			break;
		case 79:	/* aam */
			if (w1 &= 255) {
				AH = AL / w1;
				newv = AL %= w1;
			} else
				intr(0);
			OF = CF = 0;
			break;
		case 80:	/* aad */
			oprsz = 0, AX = newv = AL + w1 * AH;
			OF = CF = 0;
			break;
		case 81:	/* salc */
			AL = -CF;
			break;
		case 82:	/* xlat */
			AL = mem[16 * r[hassegpfx ? segpfx : 11] + (uint16_t) (AL + BX)];
			break;
		case 85:	/* cmc */
			CF ^= 1;
			break;
		case 86:	/* clc */
			CF = 0;
			break;
		case 87:	/* stc */
			CF = 1;
			break;
		case 88:	/* cli */
			IF = 0;
			break;
		case 89:	/* sti */
			IF = 1;
			break;
		case 90:	/* cld */
			DF = 0;
			break;
		case 91:	/* std */
			DF = 1;
			break;
		case 35:	/* test */
			POKE(AL, &, w1);
			OF = CF = 0;
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
				if (fseek(files[DL], (*(uint32_t *) &BP) << 9, SEEK_SET) != -1)
					AL = fread(&mem[16 * ES + BX], 1, AX, files[DL]);
				else
					AL = 0;
				break;
			case 3:/* write disk */
				if (fseek(files[DL], (*(uint32_t *) &BP) << 9, SEEK_SET) != -1)
					AL = fwrite(&mem[16 * ES + BX], 1, AX, files[DL]);
				else
					AL = 0;
				break;
			}
			break;
		}
		ip += (mode % 3 + 2 * !(!mode * o1a - 6)) * table20[optype] + table18[optype] - table19[optype] * ~!!oprsz;
		if (table15[optype]) {
			SF = (1 & (oprsz ? *(int16_t *) &newv : newv) >> (8 * (oprsz + 1) - 1));
			ZF = !newv;
			PF = ptable[(uint8_t) newv];
		}
		if (!++counter) {
			kb = 1;
			if (ioport[0]) {
				fprintf(stderr, "HGC not supported\n");
				exit(1);
			}
		}
	}
}
