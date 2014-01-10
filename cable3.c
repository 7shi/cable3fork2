#include <stdint.h>
#include <SDL.h>

#define R(O,M,_)(S=L?*(uint16_t*)&O:O,N=L?*(uint16_t*)&O M(f=*(uint16_t*)&_):(O M(f=*(uint8_t*)&_)))

#define P 0xf0000
uint8_t mem[1 << 21];
uint8_t *const E = &mem[P];
uint16_t *const i = &mem[P];

uint8_t t, l[80186], m, u, L, a, T, o, X, *Y, b, Q = 0, R = 0;
uint16_t M, p;
uint32_t *localtime(), f, S, kb = 0, h, W, U, c, g, d, V, A;
int N, O, j[5];
SDL_Surface *k = 0;

#define ES i[8]
#define CS i[9]
#define SS i[10]
#define DS i[11]

int
K(int o)
{
	return P + (L ? 2 * o : 2 * o + o / 4 & 7);
}

int
D(int o)
{
	return mem[*(uint32_t *) & E[259 + 4 * o] + O];
}

int
w(int o)
{
	return i[o] += ~(-2 * E[47]) * ~L;
}

int
v(int o)
{
	z((f ^= S ^ N) & 16);
	return G(N - S && 1 & (E[40] ^ f >> 8 * -~L - 1));
}

int
J(void)
{
	V = 61442;
	for (O = 9; O--;)
		V += 40[E + O] << D(25);
}

int
H(int o)
{
	u = 76;
	J();
	R(mem[16 * SS + (uint16_t) (--i[L = 4])], =, V);
	R(mem[16 * SS + (uint16_t) (--i[L = 4])], =, CS);
	R(mem[16 * SS + (uint16_t) (--i[L = 4])], =, M);
	R(mem[P + 18], =, mem[4 * o + 2]);
	R(M, =, mem[4 * o]);
	return (E[46] = 0);
}

int
s(int o)
{
	for (O = 9; O--;)
		40[E + O] = 1 && 1 << D(25) & o;
}

int
BP(int o)
{
	*i += 262 * o * z(F((*E & 15) > 9 | E[42]));
	return (*E &= 15);
}

int
SP(int o)
{
	w(7);
	return (R && --i[1] && o ? R++, Q && Q++, M-- : 0);
}

int
main(int argc, char *argv[])
{
	CS = P >> 4;
	uint16_t q = 3;
	for (O = 9; q;)
		j[--q] = *++argv ? open(*argv, 0x8082) : 0;
	*(uint32_t *) i = *j ? lseek(*j, 0, 2) >> 9 : 0;
	read(j[2], E + (M = 256), P);
	for (O = 9; Y = mem + 16 * CS + M, Y - mem; Q | R || kb & E[46] && KB) {
		L = (X = *Y & 7) & 1;
		o = X / 2 & 1;
		O = l[32] = 0;
		t = (c = *(int16_t *) & Y[++O]) & 7;
		a = c / 8 & 7;
		T = Y[1] >> 6;
		g = ~-T ? *(int16_t *) & Y[++O] : (int8_t) * (int16_t *) & Y[++O];
		d = argc = *(int16_t *) & Y[++O];
		--l[64];
		!T * t - 6 && T - 2 ? T - 1 ? d = g : 0 : (d = *(int16_t *) & Y[++O]);
		Q && Q--;
		R && R--;
		A = 4 * !T;
		O = t;
		W = h = T < 3 ? 16 * i[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[i] + D(A + 2) * g + i[D(A)]) : K(t);
		U = V = K(a);
		o ? U = h, W = V : V;
		O = *Y;
		O = u = D(51);
		uint8_t e = D(8);
		m = D(14);
		switch (e) {
		case 0:
			O = *Y / 2 & 7, M += (int8_t) c *(L ^ (D(m)[E] | D(22)[E] | D(23)[E] ^ D(24)[E]));
			break;
		case 1:
			L = *Y & 8, R(K(X)[mem], =, c);
			break;
		case 2:
			L = 2, o = 0, a = X, A = 4 * !T, O = t, W = h = T < 3 ? 16 * i[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[i] + D(A + 2) * g + i[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, a = m;
		case 5:
			a < 2 ? R(mem[U], +=1 - 2 * a +, mem[P + 24]), v(f = 1), G(S + 1 - a == 1 << 8 * -~L - 1), u = u & 4 ? 19 : 57 : a - 6 ? M += (T % 3 + 2 * !(!T * t - 6)) + 2, a - 3 || R(mem[16 * SS + (uint16_t) (--i[L = 4])], =, CS), a & 2 && R(mem[16 * SS + (uint16_t) (--i[L = 4])], =, M), a & 1 && R(mem[P + 18], =, mem[U + 2]), R(M, =, U[mem]), u = 67 : R(mem[16 * SS + (uint16_t) (--i[L = 4])], =, h[mem]);
			break;
		case 3:
			R(mem[16 * SS + (uint16_t) (--i[L = 4])], =, X[i]);
			break;
		case 4:
			(i[L = 4] += 2, R(X[i], =, mem[16 * SS + (uint16_t) (-2 + i[4])]));
			break;
		case 6:
			(W = U), a-- || (u = m, M -= ~L, R(W[mem], &, d)), a-- || (0), a-- || (R(mem[W], = ~, mem[U])), a-- || (R(mem[W], = -, mem[U]), S = 0, u = 22, F(N > S)), a-- || (L ? (1[u = 19, L + i] = (N = *(uint16_t *) & h[mem] * (uint16_t) * i) >> 16, *i = N, G(F(N - (uint16_t) N))) : (1[u = 19, L + E] = (N = *(uint8_t *) & h[mem] * (uint8_t) * E) >> 16, *i = N, G(F(N - (uint8_t) N)))), a-- || (L ? (1[u = 19, L + i] = (N = *(int16_t *) & h[mem] * (int16_t) * i) >> 16, *i = N, G(F(N - (int16_t) N))) : (1[u = 19, L + E] = (N = *(int8_t *) & h[mem] * (int8_t) * E) >> 16, *i = N, G(F(N - (int8_t) N)))), a-- || (L ? (O = *(uint16_t *) & h[mem]) && !(A = (uint32_t) (V = (1[i + L] << 16) + *i) / O, A - (uint16_t) A) ? 1[i + L] = V - O * (*i = A) : H(0) : (O = *(uint8_t *) & h[mem]) && !(A = (uint16_t) (V = (1[E + L] << 16) + *i) / O, A - (uint8_t) A) ? 1[E + L] = V - O * (*E = A) : H(0)), a-- || (L ? (O = *(int16_t *) & h[mem]) && !(A = (int) (V = (1[i + L] << 16) + *i) / O, A - (int16_t) A) ? 1[i + L] = V - O * (*i = A) : H(0) : (O = *(int8_t *) & h[mem]) && !(A = (int16_t) (V = (1[E + L] << 16) + *i) / O, A - (int8_t) A) ? 1[E + L] = V - O * (*E = A) : H(0));
			break;
		case 7:
			h = P, d = c, T = 3, a = m, M--;
		case 8:
			13[W = h, i] = (o |= !L) ? (int8_t) d : d, U = P + 26, M -= ~!o, u = 17 + (m = a);
		case 9:
			(a = m), a-- || (R(mem[W], +=, mem[U]), F(N < S)), a-- || (R(mem[W], |=, mem[U])), a-- || (v(F(40[R(mem[W], +=E[40] +, mem[U]), E] & N == S | +N < +(int) S))), a-- || (v(F(40[R(mem[W], -=E[40] +, mem[U]), E] & N == S | -N < -(int) S))), a-- || (R(mem[W], &=, mem[U])), a-- || (R(mem[W], -=, mem[U]), F(N > S)), a-- || (R(mem[W], ^=, mem[U])), a-- || (R(mem[W], -, mem[U]), F(N > S)), a-- || (R(mem[W], =, mem[U]));
			break;
		case 10:
			!L ? L = a += 8, A = 4 * !T, O = t, W = h = T < 3 ? 16 * i[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[i] + D(A + 2) * g + i[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, R(mem[W], =, mem[U]) : !o ? Q = 1, R((mem[p = m, A = 4 * !T, O = t, W = h = T < 3 ? 16 * i[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[i] + D(A + 2) * g + i[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, V]), =, h) : (i[L = 4] += 2, R(h[mem], =, mem[16 * SS + (uint16_t) (-2 + i[4])]));
			break;
		case 11:
			T = a = 0, t = 6, g = c, A = 4 * !T, O = t, W = h = T < 3 ? 16 * i[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[i] + D(A + 2) * g + i[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, R(mem[U], =, mem[W]);
			break;
		case 12:
			(A = (1 & (L ? *(int16_t *) & h[mem] : h[mem]) >> 8 * -~L - 1), V = m ? ++M, (int8_t) g : o ? 31 & E[2] : 1) && (a < 4 ? V %= a / 2 + 8 * -~L, R(A, =, h[mem]) : 0, a & 1 ? R(h[mem], >>=, V) : R(h[mem], <<=, V), a > 3 ? u = 19 : 0, a < 5 ? 0 : F(S >> V - 1 & 1)), a-- || (R(h[mem], +=, A >> 8 * -~L - V), G((1 & (L ? *(int16_t *) & N : N) >> 8 * -~L - 1) ^ F(N & 1))), a-- || (A &= (1 << V) - 1, R(h[mem], +=, A << 8 * -~L - V), G((1 & (L ? *(int16_t *) & N * 2 : N * 2) >> 8 * -~L - 1) ^ F((1 & (L ? *(int16_t *) & N : N) >> 8 * -~L - 1)))), a-- || (R(h[mem], +=(E[40] << V - 1) +, A >> 1 + 8 * -~L - V), G((1 & (L ? *(int16_t *) & N : N) >> 8 * -~L - 1) ^ F(A & 1 << 8 * -~L - V))), a-- || (R(h[mem], +=(E[40] << 8 * -~L - V) +, A << 1 + 8 * -~L - V), F(A & 1 << V - 1), G((1 & (L ? *(int16_t *) & N : N) >> 8 * -~L - 1) ^ (1 & (L ? *(int16_t *) & N * 2 : N * 2) >> 8 * -~L - 1))), a-- || (G((1 & (L ? *(int16_t *) & N : N) >> 8 * -~L - 1) ^ F((1 & (L ? *(int16_t *) & S << V - 1 : S << V - 1) >> 8 * -~L - 1)))), a-- || (G((1 & (L ? *(int16_t *) & S : S) >> 8 * -~L - 1))), a-- || (0), a-- || (V < 8 * -~L || F(A), G(0), R(h[mem], +=, A *= ~((1 << 8 * -~L) - 1 >> V)));
			break;
		case 13:
			(V = !!--1[a = X, i]), a-- || (V &= !m[E]), a-- || (V &= m[E]), a-- || (0), a-- || (V = !++i[1]), M += V * (int8_t) c;
			break;
		case 14:
			M += 3 - o, L ? 0 : o ? 9[M = 0, i] = argc : R(mem[16 * SS + (uint16_t) (--i[L = 4])], =, M), M += o * L ? (int8_t) c : c;
			break;
		case 15:
			R(mem[U], &, mem[W]);
			break;
		case 16:
			L = 7, W = P, U = K(X);
		case 24:
			W - U ? R(mem[W], ^=, mem[U]), R(mem[U], ^=, mem[W]), R(mem[W], ^=, mem[U]) : 0;
			break;
		case 17:
			!R || i[1] ? R(mem[m < 2 ? 16 * ES + (uint16_t) (i[7]) : P], =, mem[m & 1 ? P : 16 * i[Q ? p : 11] + (uint16_t) (i[6])]), m & 1 || w(6), m & 2 || SP(1) : 0;
			break;
		case 18:
			!R || i[1] ? R(mem[m ? P : 16 * i[Q ? p : 11] + (uint16_t) (i[6])], -, mem[16 * ES + (uint16_t) (i[7])]), 43[u = 92, E] = !N, F(N > S), m || w(6), SP(!N == b) : 0;
			break;
		case 19:
			o = L, (i[L = 4] += 2, R(M, =, mem[16 * SS + (uint16_t) (-2 + i[4])])), m && (i[L = 4] += 2, R(CS, =, mem[16 * SS + (uint16_t) (-2 + i[4])])), m & 2 ? s((i[L = 4] += 2, R(V, =, mem[16 * SS + (uint16_t) (-2 + i[4])]))) : o || (i[4] += c);
			break;
		case 20:
			R(U[mem], =, d);
			break;
		case 21:
			l[986] ^= 9, R(*E, =, l[m ? i[2] : (int8_t) c]);
			break;
		case 22:
			R(l[m ? i[2] : (int8_t) c], =, *E);
			break;
		case 23:
			R = 2, b = L, Q && Q++;
			break;
		case 25:
			R(mem[16 * SS + (uint16_t) (--i[L = 4])], =, m[i]);
			break;
		case 26:
			(i[L = 4] += 2, R(m[i], =, mem[16 * SS + (uint16_t) (-2 + i[4])]));
			break;
		case 27:
			Q = 2, p = m, R && R++;
			break;
		case 28:
			L = 0, O = *E, F(D(m += 3 * E[42] + 6 * E[40])), z(D(1 + m)), N = *E = D(m - 1);
			break;
		case 29:
			N = BP(m - 1);
			break;
		case 30:
			E[1] = -(1 & (L ? *(int16_t *) E : *E) >> 8 * -~L - 1);
			break;
		case 31:
			i[2] = -(1 & (L ? *(int16_t *) i : *i) >> 8 * -~L - 1);
			break;
		case 32:
			9[R(mem[16 * SS + (uint16_t) (--i[L = 4])], =, CS), R(mem[16 * SS + (uint16_t) (--i[L = 4])], =, M + 5), i] = argc, M = c;
			break;
		case 33:
			J(), R(mem[16 * SS + (uint16_t) (--i[L = 4])], =, V);
			break;
		case 34:
			s((i[L = 4] += 2, R(V, =, mem[16 * SS + (uint16_t) (-2 + i[4])])));
			break;
		case 35:
			J(), s((V & ~m) + E[1]);
			break;
		case 36:
			J(), E[1] = V;
			break;
		case 37:
			L = o = 1, A = 4 * !T, O = t, W = h = T < 3 ? 16 * i[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[i] + D(A + 2) * g + i[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, R(mem[W], =, mem[U]), R(mem[P + m], =, mem[h + 2]);
			break;
		case 38:
			++M, H(3);
			break;
		case 39:
			M += 2, H(c & m);
			break;
		case 40:
			++M, m[E] && H(4);
			break;
		case 41:
			(c &= m) ? E[1] = *E / c, N = *E %= c : H(0);
			break;
		case 42:
			*i = N = m & E[L = 0] + c * E[1];
			break;
		case 43:
			*E = -m[E];
			break;
		case 44:
			*E = mem[16 * i[Q ? p : m] + (uint16_t) (*E + i[3])];
			break;
		case 45:
			m[E] ^= 1;
			break;
		case 46:
			E[m / 2] = m & 1;
			break;
		case 47:
			R(*E, &, c);
			break;
		case 48:
			(a = c), a-- || (write(1, E, 1)), a-- || (time(j + 3), memcpy(mem + 16 * ES + (uint16_t) (i[3]), localtime(j + 3), m)), a < 2 ? *E = ~lseek(O = E[4][j], *(uint32_t *) & i[5] << 9, 0) ? (a ? write : read) (O, mem + 16 * ES + (uint16_t) (i[3]), *i) : 0 : 0;
			break;
		}
		O = u;
		D(16) ? v(0) : D(17) && G(F(0));
		M += (T % 3 + 2 * !(!T * t - 6)) * D(20) + D(18) - D(19) * ~!!L;
		D(15) ? O = m = N, 41[43[E[44] = (1 & (L ? *(int16_t *) & N : N) >> 8 * -~L - 1), E] = !N, E] = D(50) : 0;
		if (!++q) {
			kb = 1;
			if (*l) {
				SDL_PumpEvents();
				if (!k)
					k = SDL_SetVideoMode(720, 348, 32, 0);
				for (O = 9, O *= 27840; O--;)
					O[(uint32_t *) k->pixels] = -!!(1 << 7 - O % 8 & mem[O / 2880 * 90 + O % 720 / 8 + (88 + l[952] / 128 * 4 + O / 720 % 4 << 13)]);
				SDL_Flip(k);
			} else if (k) {
				SDL_Quit();
				k = 0;
			}
		}
	}
}

int
F(int o)
{
	return E[40] = !!o;
}

int
z(int o)
{
	return E[42] = !!o;
}

int
G(int o)
{
	return E[48] = o;
}
