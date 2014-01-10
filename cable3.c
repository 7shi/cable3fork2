#include <stdint.h>
#include <SDL.h>

#define R(O,M,_)(S=L?*(uint16_t*)&O:O,N=L?*(uint16_t*)&O M(f=*(uint16_t*)&_):(O M(f=*(uint8_t*)&_)))

uint8_t t, l[80186], *E, m, u, L, a, T, o, r[1 << 21], X, *Y, b, Q = 0, R = 0;
uint16_t *i, M, p, q = 3;
uint32_t *localtime(), f, S, kb = 0, h, W, U, c, g, d, V, A;
int N, O, P = 0xf0000, j[5];
SDL_Surface *k = 0;

int
K(int o)
{
	return P + (L ? 2 * o : 2 * o + o / 4 & 7);
}

int
D(int o)
{
	return r[*(uint32_t *) & E[259 + 4 * o] + O];
}

int
w(int o)
{
	return i[o] += ~(-2 * 47[E]) * ~L;
}

int
v(int o)
{
	z((f ^= S ^ N) & 16);
	return G(N - S && 1 & (40[E] ^ f >> 8 * -~L - 1));
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
	R(r[16 * i[10] + (uint16_t) (--i[L = 4])], =, V);
	R(r[16 * i[10] + (uint16_t) (--i[L = 4])], =, 9[i]);
	R(r[16 * i[10] + (uint16_t) (--i[L = 4])], =, M);
	R(r[P + 18], =, r[4 * o + 2]);
	R(M, =, r[4 * o]);
	return (46[E] = 0);
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
	*i += 262 * o * z(F((*E & 15) > 9 | 42[E]));
	return (*E &= 15);
}

int
SP(int o)
{
	w(7);
	return (R && --1[i] && o ? R++, Q && Q++, M-- : 0);
}

int
main(int argc, char *argv[])
{
	9[i = E = r + P] = P >> 4;
	for (O = 9; q;)
		j[--q] = *++argv ? open(*argv, 0x8082) : 0;
	read(2[*(uint32_t *) & *i = *j ? lseek(*j, 0, 2) >> 9 : 0, j], E + (M = 256), P);
	for (O = 9; Y = r + 16 * 9[i] + M, Y - r; Q | R || kb & 46[E] && KB) {
		L = (X = *Y & 7) & 1;
		o = X / 2 & 1;
		O = 32[l] = 0;
		t = (c = *(int16_t *) & Y[++O]) & 7;
		a = c / 8 & 7;
		T = 1[Y] >> 6;
		g = ~-T ? *(int16_t *) & Y[++O] : (int8_t) * (int16_t *) & Y[++O];
		d = argc = *(int16_t *) & Y[++O];
		--64[l];
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
			L = *Y & 8, R(K(X)[r], =, c);
			break;
		case 2:
			L = 2, o = 0, a = X, A = 4 * !T, O = t, W = h = T < 3 ? 16 * i[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[i] + D(A + 2) * g + i[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, a = m;
		case 5:
			a < 2 ? R(r[U], +=1 - 2 * a +, r[P + 24]), v(f = 1), G(S + 1 - a == 1 << 8 * -~L - 1), u = u & 4 ? 19 : 57 : a - 6 ? M += (T % 3 + 2 * !(!T * t - 6)) + 2, a - 3 || R(r[16 * i[10] + (uint16_t) (--i[L = 4])], =, 9[i]), a & 2 && R(r[16 * i[10] + (uint16_t) (--i[L = 4])], =, M), a & 1 && R(r[P + 18], =, r[U + 2]), R(M, =, U[r]), u = 67 : R(r[16 * i[10] + (uint16_t) (--i[L = 4])], =, h[r]);
			break;
		case 3:
			R(r[16 * i[10] + (uint16_t) (--i[L = 4])], =, X[i]);
			break;
		case 4:
			(i[L = 4] += 2, R(X[i], =, r[16 * i[10] + (uint16_t) (-2 + i[4])]));
			break;
		case 6:
			(W = U), a-- || (u = m, M -= ~L, R(W[r], &, d)), a-- || (0), a-- || (R(r[W], = ~, r[U])), a-- || (R(r[W], = -, r[U]), S = 0, u = 22, F(N > S)), a-- || (L ? (1[u = 19, L + i] = (N = *(uint16_t *) & h[r] * (uint16_t) * i) >> 16, *i = N, G(F(N - (uint16_t) N))) : (1[u = 19, L + E] = (N = *(uint8_t *) & h[r] * (uint8_t) * E) >> 16, *i = N, G(F(N - (uint8_t) N)))), a-- || (L ? (1[u = 19, L + i] = (N = *(int16_t *) & h[r] * (int16_t) * i) >> 16, *i = N, G(F(N - (int16_t) N))) : (1[u = 19, L + E] = (N = *(int8_t *) & h[r] * (int8_t) * E) >> 16, *i = N, G(F(N - (int8_t) N)))), a-- || (L ? (O = *(uint16_t *) & h[r]) && !(A = (uint32_t) (V = (1[i + L] << 16) + *i) / O, A - (uint16_t) A) ? 1[i + L] = V - O * (*i = A) : H(0) : (O = *(uint8_t *) & h[r]) && !(A = (uint16_t) (V = (1[E + L] << 16) + *i) / O, A - (uint8_t) A) ? 1[E + L] = V - O * (*E = A) : H(0)), a-- || (L ? (O = *(int16_t *) & h[r]) && !(A = (int) (V = (1[i + L] << 16) + *i) / O, A - (int16_t) A) ? 1[i + L] = V - O * (*i = A) : H(0) : (O = *(int8_t *) & h[r]) && !(A = (int16_t) (V = (1[E + L] << 16) + *i) / O, A - (int8_t) A) ? 1[E + L] = V - O * (*E = A) : H(0));
			break;
		case 7:
			h = P, d = c, T = 3, a = m, M--;
		case 8:
			13[W = h, i] = (o |= !L) ? (int8_t) d : d, U = P + 26, M -= ~!o, u = 17 + (m = a);
		case 9:
			(a = m), a-- || (R(r[W], +=, r[U]), F(N < S)), a-- || (R(r[W], |=, r[U])), a-- || (v(F(40[R(r[W], +=40[E] +, r[U]), E] & N == S | +N < +(int) S))), a-- || (v(F(40[R(r[W], -=40[E] +, r[U]), E] & N == S | -N < -(int) S))), a-- || (R(r[W], &=, r[U])), a-- || (R(r[W], -=, r[U]), F(N > S)), a-- || (R(r[W], ^=, r[U])), a-- || (R(r[W], -, r[U]), F(N > S)), a-- || (R(r[W], =, r[U]));
			break;
		case 10:
			!L ? L = a += 8, A = 4 * !T, O = t, W = h = T < 3 ? 16 * i[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[i] + D(A + 2) * g + i[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, R(r[W], =, r[U]) : !o ? Q = 1, R((r[p = m, A = 4 * !T, O = t, W = h = T < 3 ? 16 * i[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[i] + D(A + 2) * g + i[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, V]), =, h) : (i[L = 4] += 2, R(h[r], =, r[16 * i[10] + (uint16_t) (-2 + i[4])]));
			break;
		case 11:
			T = a = 0, t = 6, g = c, A = 4 * !T, O = t, W = h = T < 3 ? 16 * i[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[i] + D(A + 2) * g + i[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, R(r[U], =, r[W]);
			break;
		case 12:
			(A = (1 & (L ? *(int16_t *) & h[r] : h[r]) >> 8 * -~L - 1), V = m ? ++M, (int8_t) g : o ? 31 & 2[E] : 1) && (a < 4 ? V %= a / 2 + 8 * -~L, R(A, =, h[r]) : 0, a & 1 ? R(h[r], >>=, V) : R(h[r], <<=, V), a > 3 ? u = 19 : 0, a < 5 ? 0 : F(S >> V - 1 & 1)), a-- || (R(h[r], +=, A >> 8 * -~L - V), G((1 & (L ? *(int16_t *) & N : N) >> 8 * -~L - 1) ^ F(N & 1))), a-- || (A &= (1 << V) - 1, R(h[r], +=, A << 8 * -~L - V), G((1 & (L ? *(int16_t *) & N * 2 : N * 2) >> 8 * -~L - 1) ^ F((1 & (L ? *(int16_t *) & N : N) >> 8 * -~L - 1)))), a-- || (R(h[r], +=(40[E] << V - 1) +, A >> 1 + 8 * -~L - V), G((1 & (L ? *(int16_t *) & N : N) >> 8 * -~L - 1) ^ F(A & 1 << 8 * -~L - V))), a-- || (R(h[r], +=(40[E] << 8 * -~L - V) +, A << 1 + 8 * -~L - V), F(A & 1 << V - 1), G((1 & (L ? *(int16_t *) & N : N) >> 8 * -~L - 1) ^ (1 & (L ? *(int16_t *) & N * 2 : N * 2) >> 8 * -~L - 1))), a-- || (G((1 & (L ? *(int16_t *) & N : N) >> 8 * -~L - 1) ^ F((1 & (L ? *(int16_t *) & S << V - 1 : S << V - 1) >> 8 * -~L - 1)))), a-- || (G((1 & (L ? *(int16_t *) & S : S) >> 8 * -~L - 1))), a-- || (0), a-- || (V < 8 * -~L || F(A), G(0), R(h[r], +=, A *= ~((1 << 8 * -~L) - 1 >> V)));
			break;
		case 13:
			(V = !!--1[a = X, i]), a-- || (V &= !m[E]), a-- || (V &= m[E]), a-- || (0), a-- || (V = !++1[i]), M += V * (int8_t) c;
			break;
		case 14:
			M += 3 - o, L ? 0 : o ? 9[M = 0, i] = argc : R(r[16 * i[10] + (uint16_t) (--i[L = 4])], =, M), M += o * L ? (int8_t) c : c;
			break;
		case 15:
			R(r[U], &, r[W]);
			break;
		case 16:
			L = 7, W = P, U = K(X);
		case 24:
			W - U ? R(r[W], ^=, r[U]), R(r[U], ^=, r[W]), R(r[W], ^=, r[U]) : 0;
			break;
		case 17:
			!R || 1[i] ? R(r[m < 2 ? 16 * i[8] + (uint16_t) (i[7]) : P], =, r[m & 1 ? P : 16 * i[Q ? p : 11] + (uint16_t) (i[6])]), m & 1 || w(6), m & 2 || SP(1) : 0;
			break;
		case 18:
			!R || 1[i] ? R(r[m ? P : 16 * i[Q ? p : 11] + (uint16_t) (i[6])], -, r[16 * i[8] + (uint16_t) (i[7])]), 43[u = 92, E] = !N, F(N > S), m || w(6), SP(!N == b) : 0;
			break;
		case 19:
			o = L, (i[L = 4] += 2, R(M, =, r[16 * i[10] + (uint16_t) (-2 + i[4])])), m && (i[L = 4] += 2, R(9[i], =, r[16 * i[10] + (uint16_t) (-2 + i[4])])), m & 2 ? s((i[L = 4] += 2, R(V, =, r[16 * i[10] + (uint16_t) (-2 + i[4])]))) : o || (4[i] += c);
			break;
		case 20:
			R(U[r], =, d);
			break;
		case 21:
			986[l] ^= 9, R(*E, =, l[m ? 2[i] : (int8_t) c]);
			break;
		case 22:
			R(l[m ? 2[i] : (int8_t) c], =, *E);
			break;
		case 23:
			R = 2, b = L, Q && Q++;
			break;
		case 25:
			R(r[16 * i[10] + (uint16_t) (--i[L = 4])], =, m[i]);
			break;
		case 26:
			(i[L = 4] += 2, R(m[i], =, r[16 * i[10] + (uint16_t) (-2 + i[4])]));
			break;
		case 27:
			Q = 2, p = m, R && R++;
			break;
		case 28:
			L = 0, O = *E, F(D(m += 3 * 42[E] + 6 * 40[E])), z(D(1 + m)), N = *E = D(m - 1);
			break;
		case 29:
			N = BP(m - 1);
			break;
		case 30:
			1[E] = -(1 & (L ? *(int16_t *) & *E : *E) >> 8 * -~L - 1);
			break;
		case 31:
			2[i] = -(1 & (L ? *(int16_t *) & *i : *i) >> 8 * -~L - 1);
			break;
		case 32:
			9[R(r[16 * i[10] + (uint16_t) (--i[L = 4])], =, 9[i]), R(r[16 * i[10] + (uint16_t) (--i[L = 4])], =, M + 5), i] = argc, M = c;
			break;
		case 33:
			J(), R(r[16 * i[10] + (uint16_t) (--i[L = 4])], =, V);
			break;
		case 34:
			s((i[L = 4] += 2, R(V, =, r[16 * i[10] + (uint16_t) (-2 + i[4])])));
			break;
		case 35:
			J(), s((V & ~m) + 1[E]);
			break;
		case 36:
			J(), 1[E] = V;
			break;
		case 37:
			L = o = 1, A = 4 * !T, O = t, W = h = T < 3 ? 16 * i[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[i] + D(A + 2) * g + i[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, R(r[W], =, r[U]), R(r[P + m], =, r[h + 2]);
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
			(c &= m) ? 1[E] = *E / c, N = *E %= c : H(0);
			break;
		case 42:
			*i = N = m & E[L = 0] + c * 1[E];
			break;
		case 43:
			*E = -m[E];
			break;
		case 44:
			*E = r[16 * i[Q ? p : m] + (uint16_t) (*E + i[3])];
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
			(a = c), a-- || (write(1, E, 1)), a-- || (time(j + 3), memcpy(r + 16 * i[8] + (uint16_t) (i[3]), localtime(j + 3), m)), a < 2 ? *E = ~lseek(O = 4[E][j], *(uint32_t *) & 5[i] << 9, 0) ? (a ? write : read) (O, r + 16 * i[8] + (uint16_t) (i[3]), *i) : 0 : 0;
			break;
		}
		O = u;
		D(16) ? v(0) : D(17) && G(F(0));
		M += (T % 3 + 2 * !(!T * t - 6)) * D(20) + D(18) - D(19) * ~!!L;
		D(15) ? O = m = N, 41[43[44[E] = (1 & (L ? *(int16_t *) & N : N) >> 8 * -~L - 1), E] = !N, E] = D(50) : 0;
		if (!++q) {
			kb = 1;
			if (*l) {
				SDL_PumpEvents();
				if (!k)
					k = SDL_SetVideoMode(720, 348, 32, 0);
				for (O = 9, O *= 27840; O--;)
					O[(uint32_t *) k->pixels] = -!!(1 << 7 - O % 8 & r[O / 2880 * 90 + O % 720 / 8 + (88 + 952[l] / 128 * 4 + O / 720 % 4 << 13)]);
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
	return 40[E] = !!o;
}

int
z(int o)
{
	return 42[E] = !!o;
}

int
G(int o)
{
	return 48[E] = o;
}
