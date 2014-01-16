#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <SDL.h>

#define POKE(dst,opr,src) (oldv=L?*(uint16_t*)&dst:dst,newv=L?*(uint16_t*)&dst opr(srcv=*(uint16_t*)&src):(dst opr(srcv=*(uint8_t*)&src)))

#define ROMBASE 0xf0000
uint8_t mem[0x200000 /* 2MB */ ], ioport[0x10000];
uint8_t *const r8 = &mem[ROMBASE];
uint16_t *const r = (uint16_t *) &mem[ROMBASE];
uint32_t *const table = (uint32_t *) &mem[ROMBASE + 0x103];

uint8_t u, L;
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
	L = 4;
	*(uint16_t *) &mem[16 * SS + (SP -= 2)] = src;
}

uint16_t
pop(void)
{
	L = 4;
	uint16_t ret = *(uint16_t *) &mem[16 * SS + SP];
	SP += 2;
	return ret;
}

int
regmap(int reg)
{
	return ROMBASE + (L ? 2 * reg : (2 * reg + reg / 4) & 7);
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
	OF = (newv - oldv && 1 & (CF ^ srcv >> 8 * -~L - 1));
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
	u = 76;
	push(getflags());
	push(CS);
	push(ip);
	POKE(CS, =, mem[4 * n + 2]);
	POKE(ip, =, mem[4 * n]);
	IF = 0;
}

uint32_t
modrm(int mode, int t, int disp)
{
	if (mode == 3)
		return regmap(t);
	int tno = 4 * !mode;
	int seg = r[hassegpfx ? segpfx : lookup(tno + 3, t)];
	int r1 = r[lookup(tno + 1, t)];
	int hasdisp = lookup(tno + 2, t);
	int r2 = r[lookup(tno, t)];
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
	uint8_t t, a, mode, dir, reg, *ipptr, b = 0, rep = 0;
	uint16_t counter = 0;
	uint32_t kb = 0, addr, opr1, opr2, c, disp, d;
	SDL_Surface *surface = NULL;

	CS = ROMBASE >> 4, ip = 0x100;

	FILE *files[] = {NULL /* HD */ , NULL /* FD */ , NULL /* BIOS */ };
	for (int i = 1; i <= 3 && i < argc; ++i)
		files[3 - i] = fopen(argv[i], "r+b");
	if (files[0])		/* CX:AX = HDD sectors */
		*(uint32_t *) r = fseek(files[0], 0, SEEK_END) >> 9;
	fread(&mem[ROMBASE + ip], 1, ROMBASE, files[2]);	/* read BIOS */

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
		ipptr = &mem[16 * CS + ip];
		if (CS == 0 && ip == 0)
			break;
		L = (reg = *ipptr & 7) & 1;
		dir = reg / 2 & 1;
		ioport[32] = 0;
		t = (c = *(int16_t *) &ipptr[1]) & 7;
		a = c / 8 & 7;
		mode = ipptr[1] >> 6;
		disp = ~-mode ? *(int16_t *) &ipptr[2] : (int8_t) *(int16_t *) &ipptr[2];
		uint16_t w3 = d = *(int16_t *) &ipptr[3];
		--ioport[64];
		if (!mode * t != 6 && mode != 2) {
			if (mode != 1)
				d = disp;
		} else
			d = *(int16_t *) &ipptr[4];
		if (hassegpfx)
			hassegpfx--;
		if (rep)
			rep--;
		getoprs(dir, a, addr = modrm(mode, t, disp), &opr1, &opr2);
		uint8_t m = lookup(14, u = lookup(51, *ipptr));
		switch (lookup(8, u)) {
			int tmp, tmp2;
			uint32_t utmp;
		case 0:
			tmp = *ipptr / 2 & 7;
			ip += (int8_t) c *(L ^ (r8[lookup(m, tmp)] | r8[lookup(22, tmp)] | r8[lookup(23, tmp)] ^ r8[lookup(24, tmp)]));
			break;
		case 1:
			L = *ipptr & 8;
			tmp = regmap(reg);
			POKE(mem[tmp], =, c);
			break;
		case 2:
			L = 2, a = m;
			opr1 = addr = modrm(mode, t, disp), opr2 = regmap(reg);
		case 5:
			if (a < 2) {
				POKE(mem[opr2], +=1 - 2 * a +, mem[ROMBASE + 24]);
				srcv = 1;
				setafof();
				OF = (oldv + 1 - a == 1 << 8 * -~L - 1);
				u = u & 4 ? 19 : 57;
			} else if (a != 6) {
				ip += (mode % 3 + 2 * !(!mode * t - 6)) + 2;
				if (a == 3)
					push(CS);
				if (a & 2)
					push(ip);
				if (a & 1)
					POKE(mem[ROMBASE + 18], =, mem[opr2 + 2]);
				POKE(ip, =, mem[opr2]);
				u = 67;
			} else
				push(*(uint16_t *) &mem[addr]);
			break;
		case 3:
			push(r[reg]);
			break;
		case 4:
			r[reg] = pop();
			break;
		case 6:
			opr1 = opr2;
			switch (a) {
			case 0:
				u = m, ip -= ~L;
				POKE(mem[opr1], &, d);
				break;
			case 2:
				POKE(mem[opr1], = ~, mem[opr2]);
				break;
			case 3:
				POKE(mem[opr1], = -, mem[opr2]);
				oldv = 0, u = 22;
				CF = newv > oldv;
				break;
			case 4:
				u = 19;
				if (L) {
					DX = (AX = newv = *(uint16_t *) &mem[addr] * (uint16_t) *r) >> 16;
					OF = CF = !!(newv - (uint16_t) newv);
				} else {
					AX = newv = *(uint8_t *) &mem[addr] * (uint8_t) *r8;
					OF = CF = !!(newv - (uint8_t) newv);
				}
				break;
			case 5:
				u = 19;
				if (L) {
					DX = (AX = newv = *(int16_t *) &mem[addr] * (int16_t) *r) >> 16;
					OF = CF = !!(newv - (int16_t) newv);
				} else {
					AX = newv = *(int8_t *) &mem[addr] * (int8_t) *r8;
					OF = CF = !!(newv - (int8_t) newv);
				}
				break;
			case 6:
				if (L) {
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
				if (L) {
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
			addr = ROMBASE, d = c, mode = 3, a = m, ip--;
		case 8:
			opr1 = addr, opr2 = ROMBASE + 26;
			r[13] = (dir |= !L) ? (int8_t) d : d;
			ip -= ~!dir, u = 17 + (m = a);
		case 9:
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
			if (!L) {
				L = a + 8;
				getoprs(dir, L, modrm(mode, t, disp), &opr1, &opr2);
				POKE(mem[opr1], =, mem[opr2]);
			} else if (!dir) {
				hassegpfx = 1, segpfx = m, addr = modrm(mode, t, disp);
				POKE(mem[opr2], =, addr);
			} else
				*(uint16_t *) &mem[addr] = pop();
			break;
		case 11:
			getoprs(dir, 0, modrm(0, 6, c), &opr1, &opr2);
			POKE(mem[opr2], =, mem[opr1]);
			break;
		case 12:
			utmp = (1 & (L ? *(int16_t *) &mem[addr] : mem[addr]) >> 8 * -~L - 1);
			if (tmp = m ? ++ip, (int8_t) disp : dir ? 31 & CL : 1) {
				if (a < 4) {
					tmp %= a / 2 + 8 * -~L;
					POKE(utmp, =, mem[addr]);
				}
				if (a & 1)
					POKE(mem[addr], >>=, tmp);
				else
					POKE(mem[addr], <<=, tmp);
				if (a > 3)
					u = 19;
				if (a >= 5)
					CF = oldv >> tmp - 1 & 1;
			}
			switch (a) {
			case 0:
				POKE(mem[addr], +=, utmp >> 8 * -~L - tmp);
				OF = (1 & (L ? *(int16_t *) &newv : newv) >> 8 * -~L - 1) ^ (CF = newv & 1);
				break;
			case 1:
				utmp &= (1 << tmp) - 1;
				POKE(mem[addr], +=, utmp << 8 * -~L - tmp);
				CF = !!(1 & (L ? *(int16_t *) &newv : newv) >> 8 * -~L - 1);
				OF = (1 & (L ? *(int16_t *) &newv * 2 : newv * 2) >> 8 * -~L - 1) ^ CF;
				break;
			case 2:
				POKE(mem[addr], +=(CF << tmp - 1) +, utmp >> 1 + 8 * -~L - tmp);
				CF = !!(utmp & 1 << 8 * -~L - tmp);
				OF = (1 & (L ? *(int16_t *) &newv : newv) >> 8 * -~L - 1) ^ CF;
				break;
			case 3:
				POKE(mem[addr], +=(CF << 8 * -~L - tmp) +, utmp << 1 + 8 * -~L - tmp);
				CF = !!(utmp & 1 << tmp - 1);
				OF = (1 & (L ? *(int16_t *) &newv : newv) >> 8 * -~L - 1) ^ (1 & (L ? *(int16_t *) &newv * 2 : newv * 2) >> 8 * -~L - 1);
				break;
			case 4:
				CF = !!(1 & (L ? *(int16_t *) &oldv << tmp - 1 : oldv << tmp - 1) >> 8 * -~L - 1);
				OF = (1 & (L ? *(int16_t *) &newv : newv) >> 8 * -~L - 1) ^ CF;
				break;
			case 5:
				OF = (1 & (L ? *(int16_t *) &oldv : oldv) >> 8 * -~L - 1);
				break;
			case 7:
				if (tmp >= 8 * -~L)
					CF = !!utmp;
				OF = 0;
				POKE(mem[addr], +=, utmp *= ~((1 << 8 * -~L) - 1 >> tmp));
				break;
			}
			break;
		case 13:
			tmp = !!--CX;
			switch (reg) {
			case 0:
				tmp &= !r8[m];
				break;
			case 1:
				tmp &= r8[m];
				break;
			case 3:
				tmp = !++CX;
				break;
			}
			ip += tmp * (int8_t) c;
			break;
		case 14:
			ip += 3 - dir;
			if (!L) {
				if (dir)
					CS = w3, ip = 0;
				else
					push(ip);
			}
			ip += dir * L ? (int8_t) c : c;
			break;
		case 15:
			POKE(mem[opr2], &, mem[opr1]);
			break;
		case 16:
			L = 7, opr1 = ROMBASE, opr2 = regmap(reg);
		case 24:
			if (opr1 != opr2) {
				POKE(mem[opr1], ^=, mem[opr2]);
				POKE(mem[opr2], ^=, mem[opr1]);
				POKE(mem[opr1], ^=, mem[opr2]);
			}
			break;
		case 17:
			if (!rep || CX) {
				POKE(mem[m < 2 ? 16 * ES + DI : ROMBASE], =, mem[m & 1 ? ROMBASE : 16 * r[hassegpfx ? segpfx : 11] + SI]);
				tmp = ~(-2 * DF) * ~L;
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
				POKE(mem[m ? ROMBASE : 16 * r[hassegpfx ? segpfx : 11] + SI], -, mem[16 * ES + DI]);
				u = 92;
				ZF = !newv;
				CF = newv > oldv;
				tmp = ~(-2 * DF) * ~L;
				if (!m)
					SI += tmp;
				DI += tmp;
				if (rep && --CX && !newv == b) {
					rep++;
					if (hassegpfx)
						hassegpfx++;
					ip--;
				}
			}
			break;
		case 19:
			dir = L;
			ip = pop();
			if (m)
				CS = pop();
			if (m & 2)
				setflags(pop());
			else if (!dir)
				SP += c;
			break;
		case 20:
			POKE(mem[opr2], =, d);
			break;
		case 21:	/* in */
			ioport[0x3da] ^= 9;
			POKE(AL, =, ioport[m ? DX : (int8_t) c]);
			break;
		case 22:	/* out */
			POKE(ioport[m ? DX : (int8_t) c], =, AL);
			break;
		case 23:
			rep = 2, b = L;
			if (hassegpfx)
				hassegpfx++;
			break;
		case 25:
			push(r[m]);
			break;
		case 26:
			r[m] = pop();
			break;
		case 27:
			hassegpfx = 2, segpfx = m;
			if (rep)
				rep++;
			break;
		case 28:
			L = 0;
			CF = !!lookup(m += 3 * AF + 6 * CF, AL);
			AF = !!(lookup(1 + m, AL));
			newv = AL = lookup(m - 1, AL);
			break;
		case 29:
			AX += 262 * (m - 1) * (AF = CF = !!((AL & 15) > 9 | AF));
			newv = AL &= 15;
			break;
		case 30:
			AH = -(1 & (L ? *(int16_t *) r8 : AL) >> 8 * -~L - 1);
			break;
		case 31:
			DX = -(1 & (L ? *(int16_t *) r : AX) >> 8 * -~L - 1);
			break;
		case 32:	/* callf */
			push(CS);
			push(ip + 5);
			CS = w3, ip = c;
			break;
		case 33:	/* pushf */
			push(getflags());
			break;
		case 34:	/* popf */
			setflags(pop());
			break;
		case 35:	/* sahf */
			setflags((getflags() & ~m) + AH);
			break;
		case 36:	/* lahf */
			AH = getflags();
			break;
		case 37:
			L = 1;
			opr1 = regmap(a), opr2 = modrm(mode, t, disp);
			POKE(mem[opr1], =, mem[opr2]);
			POKE(mem[ROMBASE + m], =, mem[opr2 + 2]);
			break;
		case 38:	/* int3 */
			++ip;
			intr(3);
			break;
		case 39:	/* int n */
			ip += 2;
			intr(c & m);
			break;
		case 40:
			++ip;
			if (r8[m])
				intr(4);
			break;
		case 41:
			if (c &= m) {
				AH = AL / c;
				newv = AL %= c;
			} else
				intr(0);
			break;
		case 42:
			L = 0, AX = newv = m & AL + c * AH;
			break;
		case 43:
			AL = -r8[m];
			break;
		case 44:
			AL = mem[16 * r[hassegpfx ? segpfx : m] + (uint16_t) (AL + BX)];
			break;
		case 45:
			r8[m] ^= 1;
			break;
		case 46:
			r8[m / 2] = m & 1;
			break;
		case 47:
			POKE(AL, &, c);
			break;
		case 48:	/* hyper call */
			switch ((uint8_t) c) {
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
				memcpy(&mem[16 * ES + BX], localtime(&t), m);
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
		if (lookup(16, u))
			setafof();
		else if (lookup(17, u))
			OF = CF = 0;
		ip += (mode % 3 + 2 * !(!mode * t - 6)) * lookup(20, u) + lookup(18, u) - lookup(19, u) * ~!!L;
		if (lookup(15, u)) {
			SF = (1 & (L ? *(int16_t *) &newv : newv) >> 8 * -~L - 1);
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
				uint32_t *pix = (uint32_t *) surface->pixels, io = 88 + ioport[0x3b8] / 128 * 4;
				for (int y = 0, i = 0; y < 348; ++y)
					for (int x = 0; x < 720; ++x, ++i)
						pix[i] = -!!((1 << (7 - (x & 7))) & mem[y / 4 * 90 + x / 8 + ((io + (y & 3)) << 13)]);
				SDL_Flip(surface);
			} else if (surface) {
				SDL_Quit();
				surface = NULL;
			}
		}
	}
}
