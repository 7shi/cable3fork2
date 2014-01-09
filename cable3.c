#include <stdint.h>
#include <SDL.h>

uint8_t t, e, l[80186], *E, m, u, L, a, T, o, mem[1 << 21], X, *Y, b, Q = 0, R = 0;
uint16_t *r, M, p, q = 3;
uint32_t *localtime(), f, S, kb = 0, h, W, U, c, g, d, V, A;
N, O, P = 983040, j[5];
SDL_Surface *k = 0;

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
	return r[o] += ~(-2 * 47[E]) * ~L;
}

int
v(int o)
{
	return (z((f ^= S ^ N) & 16), G(N - S && 1 & (40[E] ^ f >> 8 * -~L - 1)));
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
	S = N = L = 4;
	int seg = r[10] << 4;
	r[4] -= 2;
	*(uint16_t *) & mem[seg + r[4]] = V;
	r[4] -= 2;
	*(uint16_t *) & mem[seg + r[4]] = r[9];
	r[4] -= 2;
	*(uint16_t *) & mem[seg + r[4]] = M;
	*(uint16_t *) & mem[P + 18] = *(uint16_t *) & mem[4 * o + 2];
	*(uint16_t *) & M = (f = *(uint16_t *) & mem[4 * o]);
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
	return (*r += 262 * o * z(F((*E & 15) > 9 | 42[E])), *E &= 15);
}

int
SP(int o)
{
	return (w(7), R && --1[r] && o ? R++, Q && Q++, M-- : 0);
}

int
DX(void)
{
	for (O = 9, O *= 27840; O--;)
		O[(uint32_t *) k->pixels] = -!!(1 << 7 - O % 8 & mem[O / 2880 * 90 + O % 720 / 8 + (88 + 952[l] / 128 * 4 + O / 720 % 4 << 13)]);
	SDL_Flip(k);
}

int
main(int BX, char **nE)
{
	9[r = E = mem + P] = P >> 4;
	for (O = 9; q;)
		j[--q] = *++nE ? open(*nE, 32898) : 0;
	read(2[*(uint32_t *) & *r = *j ? lseek(*j, 0, 2) >> 9 : 0, j], E + (M = 256), P);
	for (O = 9; Y = mem + 16 * 9[r] + M, Y - mem; Q | R || kb & 46[E] && KB) {
		L = (X = *Y & 7) & 1;
		o = X / 2 & 1;
		O = 32[l] = 0;
		t = (c = *(short *) &Y[++O]) & 7;
		a = c / 8 & 7;
		T = 1[Y] >> 6;
		g = ~-T ? *(short *) &Y[++O] : (char) *(short *) &Y[++O];
		d = BX = *(short *) &Y[++O];
		--64[l];
		if (!T * t - 6 && T - 2) {
			if (T - 1)
				d = g;
		} else
			d = *(short *) &Y[++O];
		Q && Q--;
		R && R--;
		A = 4 * !T;
		O = t;
		W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t);
		U = V = K(a);
		if (o) {
			U = h;
			W = V;
		}
		O = *Y;
		O = u = D(51);
		e = D(8);
		m = D(14);
		e-- || (O = *Y / 2 & 7, M += (char) c * (L ^ (D(m)[E] | D(22)[E] | D(23)[E] ^ D(24)[E])));
		e-- || (L = *Y & 8, (S = L ? *(uint16_t *) & K(X)[mem] : K(X)[mem], N = L ? *(uint16_t *) & K(X)[mem] = (f = *(uint16_t *) & c) : (K(X)[mem] = (f = *(uint8_t *) & c))));
		e-- || (L = e += 3, o = 0, a = X, A = 4 * !T, O = t, W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, a = m);
		e-- || ((S = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] : mem[16 * r[10] + (uint16_t) (--r[L = 4])], N = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint16_t *) & X[r]) : (mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint8_t *) & X[r]))));
		e-- || ((r[L = 4] += 2, (S = L ? *(uint16_t *) & X[r] : X[r], N = L ? *(uint16_t *) & X[r] = (f = *(uint16_t *) & mem[16 * r[10] + (uint16_t) (-2 + r[4])]) : (X[r] = (f = *(uint8_t *) & mem[16 * r[10] + (uint16_t) (-2 + r[4])])))));
		e-- || (a < 2 ? (S = L ? *(uint16_t *) & mem[U] : mem[U], N = L ? *(uint16_t *) & mem[U] += 1 - 2 * a + (f = *(uint16_t *) & mem[P + 24]) : (mem[U] += 1 - 2 * a + (f = *(uint8_t *) & mem[P + 24]))), v(f = 1), G(S + 1 - a == 1 << 8 * -~L - 1), u = u & 4 ? 19 : 57 : a - 6 ? M += (T % 3 + 2 * !(!T * t - 6)) + 2, a - 3 || (S = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] : mem[16 * r[10] + (uint16_t) (--r[L = 4])], N = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint16_t *) & 9[r]) : (mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint8_t *) & 9[r]))), a & 2 && (S = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] : mem[16 * r[10] + (uint16_t) (--r[L = 4])], N = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint16_t *) & M) : (mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint8_t *) & M))), a & 1 && (S = L ? *(uint16_t *) & mem[P + 18] : mem[P + 18], N = L ? *(uint16_t *) & mem[P + 18] = (f = *(uint16_t *) & mem[U + 2]) : (mem[P + 18] = (f = *(uint8_t *) & mem[U + 2]))), (S = L ? *(uint16_t *) & M : M, N = L ? *(uint16_t *) & M = (f = *(uint16_t *) & U[mem]) : (M = (f = *(uint8_t *) & U[mem]))), u = 67 : (S = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] : mem[16 * r[10] + (uint16_t) (--r[L = 4])], N = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint16_t *) & h[mem]) : (mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint8_t *) & h[mem]))));
		e-- || ((W = U), a-- || (u = m, M -= ~L, (S = L ? *(uint16_t *) & W[mem] : W[mem], N = L ? *(uint16_t *) & W[mem] & (f = *(uint16_t *) & d) : (W[mem] & (f = *(uint8_t *) & d)))), a-- || (0), a-- || ((S = L ? *(uint16_t *) & mem[W] : mem[W], N = L ? *(uint16_t *) & mem[W] = ~(f = *(uint16_t *) & mem[U]) : (mem[W] = ~(f = *(uint8_t *) & mem[U])))), a-- || ((S = L ? *(uint16_t *) & mem[W] : mem[W], N = L ? *(uint16_t *) & mem[W] = -(f = *(uint16_t *) & mem[U]) : (mem[W] = -(f = *(uint8_t *) & mem[U]))), S = 0, u = 22, F(N > S)), a-- || (L ? (1[u = 19, L + r] = (N = *(uint16_t *) & h[mem] * (uint16_t) * r) >> 16, *r = N, G(F(N - (uint16_t) N))) : (1[u = 19, L + E] = (N = *(uint8_t *) & h[mem] * (uint8_t) * E) >> 16, *r = N, G(F(N - (uint8_t) N)))), a-- || (L ? (1[u = 19, L + r] = (N = *(short *) &h[mem] * (short) *r) >> 16, *r = N, G(F(N - (short) N))) : (1[u = 19, L + E] = (N = *(char *) &h[mem] * (char) *E) >> 16, *r = N, G(F(N - (char) N)))), a-- || (L ? (O = *(uint16_t *) & h[mem]) && !(A = (uint32_t) (V = (1[r + L] << 16) + *r) / O, A - (uint16_t) A) ? 1[r + L] = V - O * (*r = A) : H(0) : (O = *(uint8_t *) & h[mem]) && !(A = (uint16_t) (V = (1[E + L] << 16) + *r) / O, A - (uint8_t) A) ? 1[E + L] = V - O * (*E = A) : H(0)), a-- || (L ? (O = *(short *) &h[mem]) && !(A = (int) (V = (1[r + L] << 16) + *r) / O, A - (short) A) ? 1[r + L] = V - O * (*r = A) : H(0) : (O = *(char *) &h[mem]) && !(A = (short) (V = (1[E + L] << 16) + *r) / O, A - (char) A) ? 1[E + L] = V - O * (*E = A) : H(0)));
		e-- || (++e, h = P, d = c, T = 3, a = m, M--);
		e-- || (++e, 13[W = h, r] = (o |= !L) ? (char) d : d, U = P + 26, M -= ~!o, u = 17 + (m = a));
		e-- || ((a = m), a-- || ((S = L ? *(uint16_t *) & mem[W] : mem[W], N = L ? *(uint16_t *) & mem[W] += (f = *(uint16_t *) & mem[U]) : (mem[W] += (f = *(uint8_t *) & mem[U]))), F(N < S)), a-- || ((S = L ? *(uint16_t *) & mem[W] : mem[W], N = L ? *(uint16_t *) & mem[W] |= (f = *(uint16_t *) & mem[U]) : (mem[W] |= (f = *(uint8_t *) & mem[U])))), a-- || (v(F(40[(S = L ? *(uint16_t *) & mem[W] : mem[W], N = L ? *(uint16_t *) & mem[W] += 40[E] + (f = *(uint16_t *) & mem[U]) : (mem[W] += 40[E] + (f = *(uint8_t *) & mem[U]))), E] & N == S | +N < +(int) S))), a-- || (v(F(40[(S = L ? *(uint16_t *) & mem[W] : mem[W], N = L ? *(uint16_t *) & mem[W] -= 40[E] + (f = *(uint16_t *) & mem[U]) : (mem[W] -= 40[E] + (f = *(uint8_t *) & mem[U]))), E] & N == S | -N < -(int) S))), a-- || ((S = L ? *(uint16_t *) & mem[W] : mem[W], N = L ? *(uint16_t *) & mem[W] &= (f = *(uint16_t *) & mem[U]) : (mem[W] &= (f = *(uint8_t *) & mem[U])))), a-- || ((S = L ? *(uint16_t *) & mem[W] : mem[W], N = L ? *(uint16_t *) & mem[W] -= (f = *(uint16_t *) & mem[U]) : (mem[W] -= (f = *(uint8_t *) & mem[U]))), F(N > S)), a-- || ((S = L ? *(uint16_t *) & mem[W] : mem[W], N = L ? *(uint16_t *) & mem[W] ^= (f = *(uint16_t *) & mem[U]) : (mem[W] ^= (f = *(uint8_t *) & mem[U])))), a-- || ((S = L ? *(uint16_t *) & mem[W] : mem[W], N = L ? *(uint16_t *) & mem[W] - (f = *(uint16_t *) & mem[U]) : (mem[W] - (f = *(uint8_t *) & mem[U]))), F(N > S)), a-- || ((S = L ? *(uint16_t *) & mem[W] : mem[W], N = L ? *(uint16_t *) & mem[W] = (f = *(uint16_t *) & mem[U]) : (mem[W] = (f = *(uint8_t *) & mem[U])))));
		e-- || (!L ? L = a += 8, A = 4 * !T, O = t, W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, (S = L ? *(uint16_t *) & mem[W] : mem[W], N = L ? *(uint16_t *) & mem[W] = (f = *(uint16_t *) & mem[U]) : (mem[W] = (f = *(uint8_t *) & mem[U]))) : !o ? Q = 1, (S = L ? *(uint16_t *) & mem[p = m, A = 4 * !T, O = t, W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, V] : mem[p = m, A = 4 * !T, O = t, W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, V], N = L ? *(uint16_t *) & mem[p = m, A = 4 * !T, O = t, W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, V] = (f = *(uint16_t *) & h) : (mem[p = m, A = 4 * !T, O = t, W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, V] = (f = *(uint8_t *) & h))) : (r[L = 4] += 2, (S = L ? *(uint16_t *) & h[mem] : h[mem], N = L ? *(uint16_t *) & h[mem] = (f = *(uint16_t *) & mem[16 * r[10] + (uint16_t) (-2 + r[4])]) : (h[mem] = (f = *(uint8_t *) & mem[16 * r[10] + (uint16_t) (-2 + r[4])])))));
		e-- || (T = a = 0, t = 6, g = c, A = 4 * !T, O = t, W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, (S = L ? *(uint16_t *) & mem[U] : mem[U], N = L ? *(uint16_t *) & mem[U] = (f = *(uint16_t *) & mem[W]) : (mem[U] = (f = *(uint8_t *) & mem[W]))));
		e-- || ((A = (1 & (L ? *(short *) &h[mem] : h[mem]) >> 8 * -~L - 1), V = m ? ++M, (char) g : o ? 31 & 2[E] : 1) && (a < 4 ? V %= a / 2 + 8 * -~L, (S = L ? *(uint16_t *) & A : A, N = L ? *(uint16_t *) & A = (f = *(uint16_t *) & h[mem]) : (A = (f = *(uint8_t *) & h[mem]))) : 0, a & 1 ? (S = L ? *(uint16_t *) & h[mem] : h[mem], N = L ? *(uint16_t *) & h[mem] >>= (f = *(uint16_t *) & V) : (h[mem] >>= (f = *(uint8_t *) & V))) : (S = L ? *(uint16_t *) & h[mem] : h[mem], N = L ? *(uint16_t *) & h[mem] <<= (f = *(uint16_t *) & V) : (h[mem] <<= (f = *(uint8_t *) & V))), a > 3 ? u = 19 : 0, a < 5 ? 0 : F(S >> V - 1 & 1)), a-- || ((S = L ? *(uint16_t *) & h[mem] : h[mem], N = L ? *(uint16_t *) & h[mem] += (f = *(uint16_t *) & A >> 8 * -~L - V) : (h[mem] += (f = *(uint8_t *) & A >> 8 * -~L - V))), G((1 & (L ? *(short *) &N : N) >> 8 * -~L - 1) ^ F(N & 1))), a-- || (A &= (1 << V) - 1, (S = L ? *(uint16_t *) & h[mem] : h[mem], N = L ? *(uint16_t *) & h[mem] += (f = *(uint16_t *) & A << 8 * -~L - V) : (h[mem] += (f = *(uint8_t *) & A << 8 * -~L - V))), G((1 & (L ? *(short *) &N * 2 : N * 2) >> 8 * -~L - 1) ^ F((1 & (L ? *(short *) &N : N) >> 8 * -~L - 1)))), a-- || ((S = L ? *(uint16_t *) & h[mem] : h[mem], N = L ? *(uint16_t *) & h[mem] += (40[E] << V - 1) + (f = *(uint16_t *) & A >> 1 + 8 * -~L - V) : (h[mem] += (40[E] << V - 1) + (f = *(uint8_t *) & A >> 1 + 8 * -~L - V))), G((1 & (L ? *(short *) &N : N) >> 8 * -~L - 1) ^ F(A & 1 << 8 * -~L - V))), a-- || ((S = L ? *(uint16_t *) & h[mem] : h[mem], N = L ? *(uint16_t *) & h[mem] += (40[E] << 8 * -~L - V) + (f = *(uint16_t *) & A << 1 + 8 * -~L - V) : (h[mem] += (40[E] << 8 * -~L - V) + (f = *(uint8_t *) & A << 1 + 8 * -~L - V))), F(A & 1 << V - 1), G((1 & (L ? *(short *) &N : N) >> 8 * -~L - 1) ^ (1 & (L ? *(short *) &N * 2 : N * 2) >> 8 * -~L - 1))), a-- || (G((1 & (L ? *(short *) &N : N) >> 8 * -~L - 1) ^ F((1 & (L ? *(short *) &S << V - 1 : S << V - 1) >> 8 * -~L - 1)))), a-- || (G((1 & (L ? *(short *) &S : S) >> 8 * -~L - 1))), a-- || (0), a-- || (V < 8 * -~L || F(A), G(0), (S = L ? *(uint16_t *) & h[mem] : h[mem], N = L ? *(uint16_t *) & h[mem] += (f = *(uint16_t *) & A *= ~((1 << 8 * -~L) - 1 >> V)) : (h[mem] += (f = *(uint8_t *) & A *= ~((1 << 8 * -~L) - 1 >> V))))));
		e-- || ((V = !!--1[a = X, r]), a-- || (V &= !m[E]), a-- || (V &= m[E]), a-- || (0), a-- || (V = !++1[r]), M += V * (char) c);
		e-- || (M += 3 - o, L ? 0 : o ? 9[M = 0, r] = BX : (S = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] : mem[16 * r[10] + (uint16_t) (--r[L = 4])], N = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint16_t *) & M) : (mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint8_t *) & M))), M += o * L ? (char) c : c);
		e-- || ((S = L ? *(uint16_t *) & mem[U] : mem[U], N = L ? *(uint16_t *) & mem[U] & (f = *(uint16_t *) & mem[W]) : (mem[U] & (f = *(uint8_t *) & mem[W]))));
		e-- || (L = e += 8, W = P, U = K(X));
		e-- || (!R || 1[r] ? (S = L ? *(uint16_t *) & mem[m < 2 ? 16 * r[8] + (uint16_t) (r[7]) : P] : mem[m < 2 ? 16 * r[8] + (uint16_t) (r[7]) : P], N = L ? *(uint16_t *) & mem[m < 2 ? 16 * r[8] + (uint16_t) (r[7]) : P] = (f = *(uint16_t *) & mem[m & 1 ? P : 16 * r[Q ? p : 11] + (uint16_t) (r[6])]) : (mem[m < 2 ? 16 * r[8] + (uint16_t) (r[7]) : P] = (f = *(uint8_t *) & mem[m & 1 ? P : 16 * r[Q ? p : 11] + (uint16_t) (r[6])]))), m & 1 || w(6), m & 2 || SP(1) : 0);
		e-- || (!R || 1[r] ? (S = L ? *(uint16_t *) & mem[m ? P : 16 * r[Q ? p : 11] + (uint16_t) (r[6])] : mem[m ? P : 16 * r[Q ? p : 11] + (uint16_t) (r[6])], N = L ? *(uint16_t *) & mem[m ? P : 16 * r[Q ? p : 11] + (uint16_t) (r[6])] - (f = *(uint16_t *) & mem[16 * r[8] + (uint16_t) (r[7])]) : (mem[m ? P : 16 * r[Q ? p : 11] + (uint16_t) (r[6])] - (f = *(uint8_t *) & mem[16 * r[8] + (uint16_t) (r[7])]))), 43[u = 92, E] = !N, F(N > S), m || w(6), SP(!N == b) : 0);
		e-- || (o = L, (r[L = 4] += 2, (S = L ? *(uint16_t *) & M : M, N = L ? *(uint16_t *) & M = (f = *(uint16_t *) & mem[16 * r[10] + (uint16_t) (-2 + r[4])]) : (M = (f = *(uint8_t *) & mem[16 * r[10] + (uint16_t) (-2 + r[4])])))), m && (r[L = 4] += 2, (S = L ? *(uint16_t *) & 9[r] : 9[r], N = L ? *(uint16_t *) & 9[r] = (f = *(uint16_t *) & mem[16 * r[10] + (uint16_t) (-2 + r[4])]) : (9[r] = (f = *(uint8_t *) & mem[16 * r[10] + (uint16_t) (-2 + r[4])])))), m & 2 ? s((r[L = 4] += 2, (S = L ? *(uint16_t *) & V : V, N = L ? *(uint16_t *) & V = (f = *(uint16_t *) & mem[16 * r[10] + (uint16_t) (-2 + r[4])]) : (V = (f = *(uint8_t *) & mem[16 * r[10] + (uint16_t) (-2 + r[4])]))))) : o || (4[r] += c));
		e-- || ((S = L ? *(uint16_t *) & U[mem] : U[mem], N = L ? *(uint16_t *) & U[mem] = (f = *(uint16_t *) & d) : (U[mem] = (f = *(uint8_t *) & d))));
		e-- || (986[l] ^= 9, (S = L ? *(uint16_t *) & *E : *E, N = L ? *(uint16_t *) & *E = (f = *(uint16_t *) & l[m ? 2[r] : (char) c]) : (*E = (f = *(uint8_t *) & l[m ? 2[r] : (char) c]))));
		e-- || ((S = L ? *(uint16_t *) & l[m ? 2[r] : (char) c] : l[m ? 2[r] : (char) c], N = L ? *(uint16_t *) & l[m ? 2[r] : (char) c] = (f = *(uint16_t *) & *E) : (l[m ? 2[r] : (char) c] = (f = *(uint8_t *) & *E))));
		e-- || (R = 2, b = L, Q && Q++);
		e-- || (W - U ? (S = L ? *(uint16_t *) & mem[W] : mem[W], N = L ? *(uint16_t *) & mem[W] ^= (f = *(uint16_t *) & mem[U]) : (mem[W] ^= (f = *(uint8_t *) & mem[U]))), (S = L ? *(uint16_t *) & mem[U] : mem[U], N = L ? *(uint16_t *) & mem[U] ^= (f = *(uint16_t *) & mem[W]) : (mem[U] ^= (f = *(uint8_t *) & mem[W]))), (S = L ? *(uint16_t *) & mem[W] : mem[W], N = L ? *(uint16_t *) & mem[W] ^= (f = *(uint16_t *) & mem[U]) : (mem[W] ^= (f = *(uint8_t *) & mem[U]))) : 0);
		e-- || ((S = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] : mem[16 * r[10] + (uint16_t) (--r[L = 4])], N = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint16_t *) & m[r]) : (mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint8_t *) & m[r]))));
		e-- || ((r[L = 4] += 2, (S = L ? *(uint16_t *) & m[r] : m[r], N = L ? *(uint16_t *) & m[r] = (f = *(uint16_t *) & mem[16 * r[10] + (uint16_t) (-2 + r[4])]) : (m[r] = (f = *(uint8_t *) & mem[16 * r[10] + (uint16_t) (-2 + r[4])])))));
		e-- || (Q = 2, p = m, R && R++);
		e-- || (L = 0, O = *E, F(D(m += 3 * 42[E] + 6 * 40[E])), z(D(1 + m)), N = *E = D(m - 1));
		e-- || (N = BP(m - 1));
		e-- || (1[E] = -(1 & (L ? *(short *) &*E : *E) >> 8 * -~L - 1));
		e-- || (2[r] = -(1 & (L ? *(short *) &*r : *r) >> 8 * -~L - 1));
		e-- || (9[(S = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] : mem[16 * r[10] + (uint16_t) (--r[L = 4])], N = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint16_t *) & 9[r]) : (mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint8_t *) & 9[r]))), (S = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] : mem[16 * r[10] + (uint16_t) (--r[L = 4])], N = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint16_t *) & M + 5) : (mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint8_t *) & M + 5))), r] = BX, M = c);
		e-- || (J(), (S = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] : mem[16 * r[10] + (uint16_t) (--r[L = 4])], N = L ? *(uint16_t *) & mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint16_t *) & V) : (mem[16 * r[10] + (uint16_t) (--r[L = 4])] = (f = *(uint8_t *) & V))));
		e-- || (s((r[L = 4] += 2, (S = L ? *(uint16_t *) & V : V, N = L ? *(uint16_t *) & V = (f = *(uint16_t *) & mem[16 * r[10] + (uint16_t) (-2 + r[4])]) : (V = (f = *(uint8_t *) & mem[16 * r[10] + (uint16_t) (-2 + r[4])]))))));
		e-- || (J(), s((V & ~m) + 1[E]));
		e-- || (J(), 1[E] = V);
		e-- || (L = o = 1, A = 4 * !T, O = t, W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (uint16_t) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, (S = L ? *(uint16_t *) & mem[W] : mem[W], N = L ? *(uint16_t *) & mem[W] = (f = *(uint16_t *) & mem[U]) : (mem[W] = (f = *(uint8_t *) & mem[U]))), (S = L ? *(uint16_t *) & mem[P + m] : mem[P + m], N = L ? *(uint16_t *) & mem[P + m] = (f = *(uint16_t *) & mem[h + 2]) : (mem[P + m] = (f = *(uint8_t *) & mem[h + 2]))));
		e-- || (++M, H(3));
		e-- || (M += 2, H(c & m));
		e-- || (++M, m[E] && H(4));
		e-- || ((c &= m) ? 1[E] = *E / c, N = *E %= c : H(0));
		e-- || (*r = N = m & E[L = 0] + c * 1[E]);
		e-- || (*E = -m[E]);
		e-- || (*E = mem[16 * r[Q ? p : m] + (uint16_t) (*E + r[3])]);
		e-- || (m[E] ^= 1);
		e-- || (E[m / 2] = m & 1);
		e-- || ((S = L ? *(uint16_t *) & *E : *E, N = L ? *(uint16_t *) & *E & (f = *(uint16_t *) & c) : (*E & (f = *(uint8_t *) & c))));
		e-- || ((a = c), a-- || (write(1, E, 1)), a-- || (time(j + 3), memcpy(mem + 16 * r[8] + (uint16_t) (r[3]), localtime(j + 3), m)), a < 2 ? *E = ~lseek(O = 4[E][j], *(uint32_t *) & 5[r] << 9, 0) ? (a ? write : read) (O, mem + 16 * r[8] + (uint16_t) (r[3]), *r) : 0 : 0);
		O = u;
		D(16) ? v(0) : D(17) && G(F(0));
		M += (T % 3 + 2 * !(!T * t - 6)) * D(20) + D(18) - D(19) * ~!!L;
		D(15) ? O = m = N, 41[43[44[E] = (1 & (L ? *(short *) &N : N) >> 8 * -~L - 1), E] = !N, E] = D(50) : 0;
		if (!++q) {
			kb = 1;
			if (*l) {
				SDL_PumpEvents();
				if (!k) {
					SDL_SetVideoMode(720, 348, 32, 0);
					k = DX();
				}
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
