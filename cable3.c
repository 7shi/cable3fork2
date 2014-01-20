#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <SDL.h>

#define POKE(dst,opr,src) (oldv=oprsz?*(uint16_t*)&dst:dst,newv=oprsz?*(uint16_t*)&dst opr(srcv=*(uint16_t*)&src):(dst opr(srcv=*(uint8_t*)&src)))

#define ROMBASE 0xf0000
uint8_t mem[0x200000 /* 2MB */ ], ioport[0x10000];
uint8_t *const r8 = &mem[ROMBASE];
uint16_t *const r = (uint16_t *) &mem[ROMBASE];
uint32_t *const table = (uint32_t *) &mem[ROMBASE + 0x103];

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

#define CF r8[40]
#define PF r8[41]
#define AF r8[42]
#define ZF r8[43]
#define SF r8[44]
#define TF r8[45]
#define IF r8[46]
#define DF r8[47]
#define OF r8[48]

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

int
regmap(int reg)
{
	return ROMBASE + (oprsz ? 2 * reg : (2 * reg + reg / 4) & 7);
}

int
lookup(int no, int offset)
{
	return mem[table[no] + offset];
}

void
setafof(void)
{
	AF = !!((srcv ^= oldv ^ newv) & 16);
	OF = (newv - oldv && 1 & (CF ^ srcv >> 8 * -~oprsz - 1));
}

uint16_t
getflags(void)
{
	uint16_t flags = 0xf002;
	for (int i = 0; i < 9; ++i)
		flags += r8[40 + i] << lookup(25, i);
	return flags;
}

void
setflags(uint16_t flags)
{
	for (int i = 0; i < 9; ++i)
		r8[40 + i] = !!(flags & (1 << lookup(25, i)));
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

uint32_t
modrm(int mode, int rm, int16_t disp)
{
	if (mode == 3)
		return regmap(rm);
	int tno = 4 * !mode;
	int seg = r[hassegpfx ? segpfx : lookup(tno + 3, rm)];
	int r1 = r[lookup(tno + 1, rm)];
	int hasdisp = lookup(tno + 2, rm);
	int r2 = r[lookup(tno, rm)];
	return 16 * seg + (uint16_t) (r1 + hasdisp * disp + r2);
}

void
getoprs(int dir, int reg, uint32_t addr, uint32_t *opr1, uint32_t *opr2)
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

	FILE *files[] = {NULL /* HD */ , NULL /* FD */ , NULL /* BIOS */ };
	for (int i = 1; i <= 3 && i < argc; ++i)
		files[3 - i] = fopen(argv[i], "r+b");
	if (files[0])		/* CX:AX = HDD sectors */
		*(uint32_t *) r = fseek(files[0], 0, SEEK_END) >> 9;
	fread(&mem[ROMBASE + ip], 1, ROMBASE, files[2]);	/* read BIOS */

	int hasrep = 0, rep = 0, kb = 0;
	uint16_t counter = 0;
	SDL_Surface *surface = NULL;

	for (;;) {
		if (!hassegpfx && !rep && kb && IF) {
			intr(8);
#ifdef _WIN32
			if (kb = kbhit()) {
				mem[0x4a6] = getch();
				intr(7);
			}
#else
			if (kb = read(fileno(stdin), &mem[0x4a6], 1))
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
		int16_t disp = ~-mode ? w2 : (int8_t) w2;
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
		uint32_t addr = modrm(mode, o1a, disp), opr1, opr2;
		getoprs(dir, o1b, addr, &opr1, &opr2);
		optype = lookup(51, ipptr[0]);
		uint8_t m = lookup(14, optype);
		switch (lookup(8, optype)) {
			int tmp, tmp2;
			uint32_t utmp;
		case 0:
			tmp = ipptr[0] / 2 & 7;
			ip += (int8_t) w1 *(oprsz ^ (r8[lookup(21, tmp)] | r8[lookup(22, tmp)] | r8[lookup(23, tmp)] ^ r8[lookup(24, tmp)]));
			break;
		case 1:
			oprsz = ipptr[0] & 8;
			tmp = regmap(o0);
			POKE(mem[tmp], =, w1);
			break;
		case 2:
			/* m = 0, 1 */
			oprsz = 2, o1b = m;
			opr1 = addr = modrm(mode, o1a, disp), opr2 = regmap(o0);
		case 5:
			if (o1b < 2) {
				POKE(mem[opr2], +=1 - 2 * o1b +, mem[ROMBASE + 24]);
				srcv = 1;
				setafof();
				OF = (oldv + 1 - o1b == 1 << 8 * -~oprsz - 1);
				optype = optype & 4 ? 19 : 57;
			} else if (o1b != 6) {
				ip += (mode % 3 + 2 * !(!mode * o1a - 6)) + 2;
				if (o1b == 3)
					push(CS);
				if (o1b & 2)
					push(ip);
				if (o1b & 1)
					POKE(mem[ROMBASE + 18], =, mem[opr2 + 2]);
				POKE(ip, =, mem[opr2]);
				optype = 67;
			} else
				push(*(uint16_t *) &mem[addr]);
			break;
		case 3:
			push(r[o0]);
			break;
		case 4:
			r[o0] = pop();
			break;
		case 6:
			opr1 = opr2;
			switch (o1b) {
			case 0:
				optype = 21, ip -= ~oprsz;
				POKE(mem[opr1], &, opr);
				break;
			case 2:
				POKE(mem[opr1], = ~, mem[opr2]);
				break;
			case 3:
				POKE(mem[opr1], = -, mem[opr2]);
				oldv = 0, optype = 22;
				CF = newv > oldv;
				break;
			case 4:
				optype = 19;
				if (oprsz) {
					DX = (AX = newv = *(uint16_t *) &mem[addr] * (uint16_t) *r) >> 16;
					OF = CF = !!(newv - (uint16_t) newv);
				} else {
					AX = newv = *(uint8_t *) &mem[addr] * (uint8_t) *r8;
					OF = CF = !!(newv - (uint8_t) newv);
				}
				break;
			case 5:
				optype = 19;
				if (oprsz) {
					DX = (AX = newv = *(int16_t *) &mem[addr] * (int16_t) *r) >> 16;
					OF = CF = !!(newv - (int16_t) newv);
				} else {
					AX = newv = *(int8_t *) &mem[addr] * (int8_t) *r8;
					OF = CF = !!(newv - (int8_t) newv);
				}
				break;
			case 6:
				if (oprsz) {
					if (tmp2 = *(uint16_t *) &mem[addr]) {
						utmp = (uint32_t) (tmp = (DX << 16) + AX) / tmp2;
						if (!(utmp - (uint16_t) utmp))
							DX = tmp - tmp2 * (AX = utmp);
						else
							intr(0);
					}
				} else {
					if (tmp2 = *(uint8_t *) &mem[addr]) {
						utmp = (uint16_t) (tmp = (AH << 16) + AX) / tmp2;
						if (!(utmp - (uint8_t) utmp))
							AH = tmp - tmp2 * (AL = utmp);
						else
							intr(0);
					}
				}
				break;
			case 7:
				if (oprsz) {
					if (tmp2 = *(int16_t *) &mem[addr]) {
						utmp = (int) (tmp = (DX << 16) + AX) / tmp2;
						if (!(utmp - (int16_t) utmp))
							DX = tmp - tmp2 * (AX = utmp);
						else
							intr(0);
					}
				} else {
					if (tmp2 = *(int8_t *) &mem[addr]) {
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
		case 7:
			/* m = 0, 1, 2, 3, 4, 5, 6, 7 */
			addr = ROMBASE, opr = w1, mode = 3, o1b = m, ip--;
		case 8:
			opr1 = addr, opr2 = ROMBASE + 26;
			r[13] = (dir |= !oprsz) ? (int8_t) opr : opr;
			ip -= ~!dir, optype = 17 + (m = o1b);
		case 9:
			/* m = 0, 1, 2, 3, 4, 5, 6, 7, 8 */
			switch (m) {
			case 0:
				POKE(mem[opr1], +=, mem[opr2]);
				CF = newv < oldv;
				break;
			case 1:
				POKE(mem[opr1], |=, mem[opr2]);
				break;
			case 2:
				POKE(mem[opr1], +=CF +, mem[opr2]);
				CF = !!(CF & newv == oldv | +newv < +(int) oldv);
				setafof();
				break;
			case 3:
				POKE(mem[opr1], -=CF +, mem[opr2]);
				CF = !!(CF & newv == oldv | -newv < -(int) oldv);
				setafof();
				break;
			case 4:
				POKE(mem[opr1], &=, mem[opr2]);
				break;
			case 5:
				POKE(mem[opr1], -=, mem[opr2]);
				CF = newv > oldv;
				break;
			case 6:
				POKE(mem[opr1], ^=, mem[opr2]);
				break;
			case 7:
				POKE(mem[opr1], -, mem[opr2]);
				CF = newv > oldv;
				break;
			case 8:
				POKE(mem[opr1], =, mem[opr2]);
				break;
			}
			break;
		case 10:
			if (!oprsz) {
				oprsz = o1b + 8;
				getoprs(dir, oprsz, modrm(mode, o1a, disp), &opr1, &opr2);
				POKE(mem[opr1], =, mem[opr2]);
			} else if (!dir) {
				hassegpfx = 1, segpfx = 12, addr = modrm(mode, o1a, disp);
				POKE(mem[opr2], =, addr);
			} else
				*(uint16_t *) &mem[addr] = pop();
			break;
		case 11:
			getoprs(dir, 0, modrm(0, 6, w1), &opr1, &opr2);
			POKE(mem[opr2], =, mem[opr1]);
			break;
		case 12:
			utmp = (1 & (oprsz ? *(int16_t *) &mem[addr] : mem[addr]) >> 8 * -~oprsz - 1);
			/* m = 0, 1 */
			if (tmp = m ? ++ip, (int8_t) disp : dir ? 31 & CL : 1) {
				if (o1b < 4) {
					tmp %= o1b / 2 + 8 * -~oprsz;
					POKE(utmp, =, mem[addr]);
				}
				if (o1b & 1)
					POKE(mem[addr], >>=, tmp);
				else
					POKE(mem[addr], <<=, tmp);
				if (o1b > 3)
					optype = 19;
				if (o1b >= 5)
					CF = oldv >> tmp - 1 & 1;
			}
			switch (o1b) {
			case 0:
				POKE(mem[addr], +=, utmp >> 8 * -~oprsz - tmp);
				OF = (1 & (oprsz ? *(int16_t *) &newv : newv) >> 8 * -~oprsz - 1) ^ (CF = newv & 1);
				break;
			case 1:
				utmp &= (1 << tmp) - 1;
				POKE(mem[addr], +=, utmp << 8 * -~oprsz - tmp);
				CF = !!(1 & (oprsz ? *(int16_t *) &newv : newv) >> 8 * -~oprsz - 1);
				OF = (1 & (oprsz ? *(int16_t *) &newv * 2 : newv * 2) >> 8 * -~oprsz - 1) ^ CF;
				break;
			case 2:
				POKE(mem[addr], +=(CF << tmp - 1) +, utmp >> 1 + 8 * -~oprsz - tmp);
				CF = !!(utmp & 1 << 8 * -~oprsz - tmp);
				OF = (1 & (oprsz ? *(int16_t *) &newv : newv) >> 8 * -~oprsz - 1) ^ CF;
				break;
			case 3:
				POKE(mem[addr], +=(CF << 8 * -~oprsz - tmp) +, utmp << 1 + 8 * -~oprsz - tmp);
				CF = !!(utmp & 1 << tmp - 1);
				OF = (1 & (oprsz ? *(int16_t *) &newv : newv) >> 8 * -~oprsz - 1) ^ (1 & (oprsz ? *(int16_t *) &newv * 2 : newv * 2) >> 8 * -~oprsz - 1);
				break;
			case 4:
				CF = !!(1 & (oprsz ? *(int16_t *) &oldv << tmp - 1 : oldv << tmp - 1) >> 8 * -~oprsz - 1);
				OF = (1 & (oprsz ? *(int16_t *) &newv : newv) >> 8 * -~oprsz - 1) ^ CF;
				break;
			case 5:
				OF = (1 & (oprsz ? *(int16_t *) &oldv : oldv) >> 8 * -~oprsz - 1);
				break;
			case 7:
				if (tmp >= 8 * -~oprsz)
					CF = !!utmp;
				OF = 0;
				POKE(mem[addr], +=, utmp *= ~((1 << 8 * -~oprsz) - 1 >> tmp));
				break;
			}
			break;
		case 13:
			tmp = !!--CX;
			switch (o0) {
			case 0:
				tmp &= !ZF;
				break;
			case 1:
				tmp &= ZF;
				break;
			case 3:
				tmp = !++CX;
				break;
			}
			ip += tmp * (int8_t) w1;
			break;
		case 14:
			ip += 3 - dir;
			if (!oprsz) {
				if (dir)
					CS = w3, ip = 0;
				else
					push(ip);
			}
			ip += dir * oprsz ? (int8_t) w1 : w1;
			break;
		case 15:
			POKE(mem[opr2], &, mem[opr1]);
			break;
		case 16:
			oprsz = 7, opr1 = ROMBASE, opr2 = regmap(o0);
		case 24:
			if (opr1 != opr2) {
				POKE(mem[opr1], ^=, mem[opr2]);
				POKE(mem[opr2], ^=, mem[opr1]);
				POKE(mem[opr1], ^=, mem[opr2]);
			}
			break;
		case 17:
			if (!rep || CX) {
				/* m = 0, 1, 2 */
				opr1 = m < 2 ? 16 * ES + DI : ROMBASE;
				opr2 = m & 1 ? ROMBASE : 16 * r[hassegpfx ? segpfx : 11] + SI;
				POKE(mem[opr1], =, mem[opr2]);
				tmp = ~(-2 * DF) * ~oprsz;
				if (!(m & 1))
					SI += tmp;
				if (!(m & 2)) {
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
		case 18:
			if (!rep || CX) {
				/* m = 0, 1 */
				opr1 = m ? ROMBASE : 16 * r[hassegpfx ? segpfx : 11] + SI;
				opr2 = 16 * ES + DI;
				POKE(mem[opr1], -, mem[opr2]);
				optype = 92;
				ZF = !newv;
				CF = newv > oldv;
				tmp = ~(-2 * DF) * ~oprsz;
				if (!m)
					SI += tmp;
				DI += tmp;
				if (rep && --CX && !newv == hasrep) {
					rep++;
					if (hassegpfx)
						hassegpfx++;
					ip--;
				}
			}
			break;
		case 19:
			dir = oprsz;
			ip = pop();
			/* m = 0, 1, 2 */
			if (m)
				CS = pop();
			if (m & 2)
				setflags(pop());
			else if (!dir)
				SP += w1;
			break;
		case 20:
			POKE(mem[opr2], =, opr);
			break;
		case 21:	/* in */
			ioport[0x3da] ^= 9;
			/* m = 0, 1 */
			POKE(AL, =, ioport[m ? DX : (int8_t) w1]);
			break;
		case 22:	/* out */
			/* m = 0, 1 */
			POKE(ioport[m ? DX : (int8_t) w1], =, AL);
			break;
		case 23:
			rep = 2, hasrep = oprsz;
			if (hassegpfx)
				hassegpfx++;
			break;
		case 25:	/* push seg */
			/* m = 8: ES, 9: CS, 10: SS, 11: DS */
			push(r[m]);
			break;
		case 26:	/* pop seg */
			/* m = 8: ES, 10: SS, 11: DS */
			r[m] = pop();
			break;
		case 27:
			/* m = 8: ES, 9: CS, 10: SS, 11: DS */
			hassegpfx = 2, segpfx = m;
			if (rep)
				rep++;
			break;
		case 28:
			oprsz = 0;
			/* m = 27, 39 */
			CF = !!lookup(m += 3 * AF + 6 * CF, AL);
			AF = !!(lookup(1 + m, AL));
			newv = AL = lookup(m - 1, AL);
			break;
		case 29:
			/* m = 0, 2 */
			AX += 262 * (m - 1) * (AF = CF = !!((AL & 15) > 9 | AF));
			newv = AL &= 15;
			break;
		case 30:
			AH = -(1 & (oprsz ? *(int16_t *) r8 : AL) >> 8 * -~oprsz - 1);
			break;
		case 31:
			DX = -(1 & (oprsz ? *(int16_t *) r : AX) >> 8 * -~oprsz - 1);
			break;
		case 32:	/* callf */
			push(CS);
			push(ip + 5);
			CS = w3, ip = w1;
			break;
		case 33:	/* pushf */
			push(getflags());
			break;
		case 34:	/* popf */
			setflags(pop());
			break;
		case 35:	/* sahf */
			setflags((getflags() & ~255) + AH);
			break;
		case 36:	/* lahf */
			AH = getflags();
			break;
		case 37:
			oprsz = 1;
			opr1 = regmap(o1b), opr2 = modrm(mode, o1a, disp);
			POKE(mem[opr1], =, mem[opr2]);
			/* m = 16, 22 */
			POKE(mem[ROMBASE + m], =, mem[opr2 + 2]);
			break;
		case 38:	/* int3 */
			++ip;
			intr(3);
			break;
		case 39:	/* int n */
			ip += 2;
			intr(w1 & 255);
			break;
		case 40:
			++ip;
			if (OF)
				intr(4);
			break;
		case 41:
			if (w1 &= 255) {
				AH = AL / w1;
				newv = AL %= w1;
			} else
				intr(0);
			break;
		case 42:
			oprsz = 0, AX = newv = AL + w1 * AH;
			break;
		case 43:
			AL = -CF;
			break;
		case 44:
			AL = mem[16 * r[hassegpfx ? segpfx : 11] + (uint16_t) (AL + BX)];
			break;
		case 45:
			CF ^= 1;
			break;
		case 46:
			/* m = 80-81: CF, 92-93: IF, 94-95: DF */
			r8[m / 2] = m & 1;
			break;
		case 47:
			POKE(AL, &, w1);
			break;
		case 48:	/* hyper call */
			switch ((uint8_t) w1) {
				time_t t;
#ifdef _WIN32
				static int skipcnt;
#endif
			case 0:
#ifdef _WIN32
				if (AL == 27)
					skipcnt = 6;
				if (skipcnt && skipcnt--)
					break;
#endif
				write(fileno(stdout), &AL, 1);
				break;
			case 1:
				time(&t);
				memcpy(&mem[16 * ES + BX], localtime(&t), 36);
				break;
			case 2:
				if (fseek(files[DL], (*(uint32_t *) &BP) << 9, SEEK_SET) != -1)
					AL = fread(&mem[16 * ES + BX], 1, AX, files[DL]);
				else
					AL = 0;
				break;
			case 3:
				if (fseek(files[DL], (*(uint32_t *) &BP) << 9, SEEK_SET) != -1)
					AL = fwrite(&mem[16 * ES + BX], 1, AX, files[DL]);
				else
					AL = 0;
				break;
			}
			break;
		}
		if (lookup(16, optype))
			setafof();
		else if (lookup(17, optype))
			OF = CF = 0;
		ip += (mode % 3 + 2 * !(!mode * o1a - 6)) * lookup(20, optype) + lookup(18, optype) - lookup(19, optype) * ~!!oprsz;
		if (lookup(15, optype)) {
			SF = (1 & (oprsz ? *(int16_t *) &newv : newv) >> 8 * -~oprsz - 1);
			ZF = !newv;
			PF = lookup(50, (uint8_t) newv);
		}
		if (!++counter) {
			kb = 1;
			if (ioport[0]) {
				/* Hercules Graphics Card (HGC) */
				SDL_PumpEvents();
				if (!surface)
					surface = SDL_SetVideoMode(720, 348, 32, 0);
				uint32_t *pix = (uint32_t *) surface->pixels;
				uint8_t *fb = &mem[0xb0000 + ioport[0x3b8] / 128 * 0x8000];
				for (int y = 0, i = 0; y < 348; ++y)
					for (int x = 0; x < 720; ++x, ++i)
						pix[i] = -!!((1 << (7 - (x & 7))) & fb[x / 8 + y / 4 * 90 + ((y & 3) << 13)]);
				SDL_Flip(surface);
			} else if (surface) {
				SDL_Quit();
				surface = NULL;
			}
		}
	}
}
