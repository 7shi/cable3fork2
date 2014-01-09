#include <SDL.h>

unsigned char t, e, l[80186], *E, m, u, L, a, T, o, mem[1 << 21], X, *Y, b,
     Q = 0, R = 0;
unsigned short *r, M, p, q = 3;
unsigned *localtime(), f, S, kb = 0, h, W, U, c, g, d, V, A;
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
	return mem[*(unsigned *) &E[259 + 4 * o] + O];
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
	*(unsigned short *) &mem[seg + r[4]] = V;
	r[4] -= 2;
	*(unsigned short *) &mem[seg + r[4]] = r[9];
	r[4] -= 2;
	*(unsigned short *) &mem[seg + r[4]] = M;
	*(unsigned short *) &mem[P + 18] = *(unsigned short *) &mem[4 * o + 2];
	*(unsigned short *) &M = (f = *(unsigned short *) &mem[4 * o]);
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
		O[(unsigned *) k->pixels] = -!!(1 << 7 - O % 8 & mem[O / 2880 * 90 + O % 720 / 8 + (88 + 952[l] / 128 * 4 + O / 720 % 4 << 13)]);
	SDL_Flip(k);
}

int
main(int BX, char **nE)
{
	9[r = E = mem + P] = P >> 4;
	for (O = 9; q;)
		j[--q] = *++nE ? open(*nE, 32898) : 0;
	read(2[*(unsigned *) &*r = *j ? lseek(*j, 0, 2) >> 9 : 0, j], E + (M = 256), P);
	for (O = 9; Y = mem + 16 * 9[r] + M, Y - mem; Q | R || kb & 46[E] && KB)
		--64[T = 1[O = 32[L = (X = *Y & 7) & 1, o = X / 2 & 1, l] = 0, t = (c = *(short *) &Y[++O]) & 7, a = c / 8 & 7, Y] >> 6, g = ~-T ? *(short *) &Y[++O] : (char) *(short *) &Y[++O], d = BX = *(short *) &Y[++O], l], !T * t - 6 && T - 2 ? T - 1 ? d = g : 0 : (d = *(short *) &Y[++O]), Q && Q--, R && R--, A = 4 * !T, O = t, W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (unsigned short) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, (O = *Y, O = u = D(51), e = D(8), m = D(14)), e-- || (O = *Y / 2 & 7, M += (char) c * (L ^ (D(m)[E] | D(22)[E] | D(23)[E] ^ D(24)[E]))), e-- || (L = *Y & 8, (S = L ? *(unsigned short *) &K(X)[mem] : K(X)[mem], N = L ? *(unsigned short *) &K(X)[mem] = (f = *(unsigned short *) &c) : (K(X)[mem] = (f = *(unsigned char *) &c)))), e-- || (L = e += 3, o = 0, a = X, A = 4 * !T, O = t, W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (unsigned short) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, a = m), e-- || ((S = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] : mem[16 * r[10] + (unsigned short) (--r[L = 4])], N = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned short *) &X[r]) : (mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned char *) &X[r])))), e-- || ((r[L = 4] += 2, (S = L ? *(unsigned short *) &X[r] : X[r], N = L ? *(unsigned short *) &X[r] = (f = *(unsigned short *) &mem[16 * r[10] + (unsigned short) (-2 + r[4])]) : (X[r] = (f = *(unsigned char *) &mem[16 * r[10] + (unsigned short) (-2 + r[4])]))))), e-- || (a < 2 ? (S = L ? *(unsigned short *) &mem[U] : mem[U], N = L ? *(unsigned short *) &mem[U] += 1 - 2 * a + (f = *(unsigned short *) &mem[P + 24]) : (mem[U] += 1 - 2 * a + (f = *(unsigned char *) &mem[P + 24]))), v(f = 1), G(S + 1 - a == 1 << 8 * -~L - 1), u = u & 4 ? 19 : 57 : a - 6 ? M += (T % 3 + 2 * !(!T * t - 6)) + 2, a - 3 || (S = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] : mem[16 * r[10] + (unsigned short) (--r[L = 4])], N = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned short *) &9[r]) : (mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned char *) &9[r]))), a & 2 && (S = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] : mem[16 * r[10] + (unsigned short) (--r[L = 4])], N = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned short *) &M) : (mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned char *) &M))), a & 1 && (S = L ? *(unsigned short *) &mem[P + 18] : mem[P + 18], N = L ? *(unsigned short *) &mem[P + 18] = (f = *(unsigned short *) &mem[U + 2]) : (mem[P + 18] = (f = *(unsigned char *) &mem[U + 2]))), (S = L ? *(unsigned short *) &M : M, N = L ? *(unsigned short *) &M = (f = *(unsigned short *) &U[mem]) : (M = (f = *(unsigned char *) &U[mem]))), u = 67 : (S = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] : mem[16 * r[10] + (unsigned short) (--r[L = 4])], N = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned short *) &h[mem]) : (mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned char *) &h[mem])))), e-- || ((W = U), a-- || (u = m, M -= ~L, (S = L ? *(unsigned short *) &W[mem] : W[mem], N = L ? *(unsigned short *) &W[mem] & (f = *(unsigned short *) &d) : (W[mem] & (f = *(unsigned char *) &d)))), a-- || (0), a-- || ((S = L ? *(unsigned short *) &mem[W] : mem[W], N = L ? *(unsigned short *) &mem[W] = ~(f = *(unsigned short *) &mem[U]) : (mem[W] = ~(f = *(unsigned char *) &mem[U])))), a-- || ((S = L ? *(unsigned short *) &mem[W] : mem[W], N = L ? *(unsigned short *) &mem[W] = -(f = *(unsigned short *) &mem[U]) : (mem[W] = -(f = *(unsigned char *) &mem[U]))), S = 0, u = 22, F(N > S)), a-- || (L ? (1[u = 19, L + r] = (N = *(unsigned short *) &h[mem] * (unsigned short) *r) >> 16, *r = N, G(F(N - (unsigned short) N))) : (1[u = 19, L + E] = (N = *(unsigned char *) &h[mem] * (unsigned char) *E) >> 16, *r = N, G(F(N - (unsigned char) N)))), a-- || (L ? (1[u = 19, L + r] = (N = *(short *) &h[mem] * (short) *r) >> 16, *r = N, G(F(N - (short) N))) : (1[u = 19, L + E] = (N = *(char *) &h[mem] * (char) *E) >> 16, *r = N, G(F(N - (char) N)))), a-- || (L ? (O = *(unsigned short *) &h[mem]) && !(A = (unsigned) (V = (1[r + L] << 16) + *r) / O, A - (unsigned short) A) ? 1[r + L] = V - O * (*r = A) : H(0) : (O = *(unsigned char *) &h[mem]) && !(A = (unsigned short) (V = (1[E + L] << 16) + *r) / O, A - (unsigned char) A) ? 1[E + L] = V - O * (*E = A) : H(0)), a-- || (L ? (O = *(short *) &h[mem]) && !(A = (int) (V = (1[r + L] << 16) + *r) / O, A - (short) A) ? 1[r + L] = V - O * (*r = A) : H(0) : (O = *(char *) &h[mem]) && !(A = (short) (V = (1[E + L] << 16) + *r) / O, A - (char) A) ? 1[E + L] = V - O * (*E = A) : H(0))), e-- || (++e, h = P, d = c, T = 3, a = m, M--), e-- || (++e, 13[W = h, r] = (o |= !L) ? (char) d : d, U = P + 26, M -= ~!o, u = 17 + (m = a)), e-- || ((a = m), a-- || ((S = L ? *(unsigned short *) &mem[W] : mem[W], N = L ? *(unsigned short *) &mem[W] += (f = *(unsigned short *) &mem[U]) : (mem[W] += (f = *(unsigned char *) &mem[U]))), F(N < S)), a-- || ((S = L ? *(unsigned short *) &mem[W] : mem[W], N = L ? *(unsigned short *) &mem[W] |= (f = *(unsigned short *) &mem[U]) : (mem[W] |= (f = *(unsigned char *) &mem[U])))), a-- || (v(F(40[(S = L ? *(unsigned short *) &mem[W] : mem[W], N = L ? *(unsigned short *) &mem[W] += 40[E] + (f = *(unsigned short *) &mem[U]) : (mem[W] += 40[E] + (f = *(unsigned char *) &mem[U]))), E] & N == S | +N < +(int) S))), a-- || (v(F(40[(S = L ? *(unsigned short *) &mem[W] : mem[W], N = L ? *(unsigned short *) &mem[W] -= 40[E] + (f = *(unsigned short *) &mem[U]) : (mem[W] -= 40[E] + (f = *(unsigned char *) &mem[U]))), E] & N == S | -N < -(int) S))), a-- || ((S = L ? *(unsigned short *) &mem[W] : mem[W], N = L ? *(unsigned short *) &mem[W] &= (f = *(unsigned short *) &mem[U]) : (mem[W] &= (f = *(unsigned char *) &mem[U])))), a-- || ((S = L ? *(unsigned short *) &mem[W] : mem[W], N = L ? *(unsigned short *) &mem[W] -= (f = *(unsigned short *) &mem[U]) : (mem[W] -= (f = *(unsigned char *) &mem[U]))), F(N > S)), a-- || ((S = L ? *(unsigned short *) &mem[W] : mem[W], N = L ? *(unsigned short *) &mem[W] ^= (f = *(unsigned short *) &mem[U]) : (mem[W] ^= (f = *(unsigned char *) &mem[U])))), a-- || ((S = L ? *(unsigned short *) &mem[W] : mem[W], N = L ? *(unsigned short *) &mem[W] - (f = *(unsigned short *) &mem[U]) : (mem[W] - (f = *(unsigned char *) &mem[U]))), F(N > S)), a-- || ((S = L ? *(unsigned short *) &mem[W] : mem[W], N = L ? *(unsigned short *) &mem[W] = (f = *(unsigned short *) &mem[U]) : (mem[W] = (f = *(unsigned char *) &mem[U]))))), e-- || (!L ? L = a += 8, A = 4 * !T, O = t, W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (unsigned short) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, (S = L ? *(unsigned short *) &mem[W] : mem[W], N = L ? *(unsigned short *) &mem[W] = (f = *(unsigned short *) &mem[U]) : (mem[W] = (f = *(unsigned char *) &mem[U]))) : !o ? Q = 1, (S = L ? *(unsigned short *) &mem[p = m, A = 4 * !T, O = t, W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (unsigned short) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, V] : mem[p = m, A = 4 * !T, O = t, W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (unsigned short) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, V], N = L ? *(unsigned short *) &mem[p = m, A = 4 * !T, O = t, W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (unsigned short) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, V] = (f = *(unsigned short *) &h) : (mem[p = m, A = 4 * !T, O = t, W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (unsigned short) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, V] = (f = *(unsigned char *) &h))) : (r[L = 4] += 2, (S = L ? *(unsigned short *) &h[mem] : h[mem], N = L ? *(unsigned short *) &h[mem] = (f = *(unsigned short *) &mem[16 * r[10] + (unsigned short) (-2 + r[4])]) : (h[mem] = (f = *(unsigned char *) &mem[16 * r[10] + (unsigned short) (-2 + r[4])]))))), e-- || (T = a = 0, t = 6, g = c, A = 4 * !T, O = t, W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (unsigned short) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, (S = L ? *(unsigned short *) &mem[U] : mem[U], N = L ? *(unsigned short *) &mem[U] = (f = *(unsigned short *) &mem[W]) : (mem[U] = (f = *(unsigned char *) &mem[W])))), e-- || ((A = (1 & (L ? *(short *) &h[mem] : h[mem]) >> 8 * -~L - 1), V = m ? ++M, (char) g : o ? 31 & 2[E] : 1) && (a < 4 ? V %= a / 2 + 8 * -~L, (S = L ? *(unsigned short *) &A : A, N = L ? *(unsigned short *) &A = (f = *(unsigned short *) &h[mem]) : (A = (f = *(unsigned char *) &h[mem]))) : 0, a & 1 ? (S = L ? *(unsigned short *) &h[mem] : h[mem], N = L ? *(unsigned short *) &h[mem] >>= (f = *(unsigned short *) &V) : (h[mem] >>= (f = *(unsigned char *) &V))) : (S = L ? *(unsigned short *) &h[mem] : h[mem], N = L ? *(unsigned short *) &h[mem] <<= (f = *(unsigned short *) &V) : (h[mem] <<= (f = *(unsigned char *) &V))), a > 3 ? u = 19 : 0, a < 5 ? 0 : F(S >> V - 1 & 1)), a-- || ((S = L ? *(unsigned short *) &h[mem] : h[mem], N = L ? *(unsigned short *) &h[mem] += (f = *(unsigned short *) &A >> 8 * -~L - V) : (h[mem] += (f = *(unsigned char *) &A >> 8 * -~L - V))), G((1 & (L ? *(short *) &N : N) >> 8 * -~L - 1) ^ F(N & 1))), a-- || (A &= (1 << V) - 1, (S = L ? *(unsigned short *) &h[mem] : h[mem], N = L ? *(unsigned short *) &h[mem] += (f = *(unsigned short *) &A << 8 * -~L - V) : (h[mem] += (f = *(unsigned char *) &A << 8 * -~L - V))), G((1 & (L ? *(short *) &N * 2 : N * 2) >> 8 * -~L - 1) ^ F((1 & (L ? *(short *) &N : N) >> 8 * -~L - 1)))), a-- || ((S = L ? *(unsigned short *) &h[mem] : h[mem], N = L ? *(unsigned short *) &h[mem] += (40[E] << V - 1) + (f = *(unsigned short *) &A >> 1 + 8 * -~L - V) : (h[mem] += (40[E] << V - 1) + (f = *(unsigned char *) &A >> 1 + 8 * -~L - V))), G((1 & (L ? *(short *) &N : N) >> 8 * -~L - 1) ^ F(A & 1 << 8 * -~L - V))), a-- || ((S = L ? *(unsigned short *) &h[mem] : h[mem], N = L ? *(unsigned short *) &h[mem] += (40[E] << 8 * -~L - V) + (f = *(unsigned short *) &A << 1 + 8 * -~L - V) : (h[mem] += (40[E] << 8 * -~L - V) + (f = *(unsigned char *) &A << 1 + 8 * -~L - V))), F(A & 1 << V - 1), G((1 & (L ? *(short *) &N : N) >> 8 * -~L - 1) ^ (1 & (L ? *(short *) &N * 2 : N * 2) >> 8 * -~L - 1))), a-- || (G((1 & (L ? *(short *) &N : N) >> 8 * -~L - 1) ^ F((1 & (L ? *(short *) &S << V - 1 : S << V - 1) >> 8 * -~L - 1)))), a-- || (G((1 & (L ? *(short *) &S : S) >> 8 * -~L - 1))), a-- || (0), a-- || (V < 8 * -~L || F(A), G(0), (S = L ? *(unsigned short *) &h[mem] : h[mem], N = L ? *(unsigned short *) &h[mem] += (f = *(unsigned short *) &A *= ~((1 << 8 * -~L) - 1 >> V)) : (h[mem] += (f = *(unsigned char *) &A *= ~((1 << 8 * -~L) - 1 >> V)))))), e-- || ((V = !!--1[a = X, r]), a-- || (V &= !m[E]), a-- || (V &= m[E]), a-- || (0), a-- || (V = !++1[r]), M += V * (char) c), e-- || (M += 3 - o, L ? 0 : o ? 9[M = 0, r] = BX : (S = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] : mem[16 * r[10] + (unsigned short) (--r[L = 4])], N = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned short *) &M) : (mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned char *) &M))), M += o * L ? (char) c : c), e-- || ((S = L ? *(unsigned short *) &mem[U] : mem[U], N = L ? *(unsigned short *) &mem[U] & (f = *(unsigned short *) &mem[W]) : (mem[U] & (f = *(unsigned char *) &mem[W])))), e-- || (L = e += 8, W = P, U = K(X)), e-- || (!R || 1[r] ? (S = L ? *(unsigned short *) &mem[m < 2 ? 16 * r[8] + (unsigned short) (r[7]) : P] : mem[m < 2 ? 16 * r[8] + (unsigned short) (r[7]) : P], N = L ? *(unsigned short *) &mem[m < 2 ? 16 * r[8] + (unsigned short) (r[7]) : P] = (f = *(unsigned short *) &mem[m & 1 ? P : 16 * r[Q ? p : 11] + (unsigned short) (r[6])]) : (mem[m < 2 ? 16 * r[8] + (unsigned short) (r[7]) : P] = (f = *(unsigned char *) &mem[m & 1 ? P : 16 * r[Q ? p : 11] + (unsigned short) (r[6])]))), m & 1 || w(6), m & 2 || SP(1) : 0), e-- || (!R || 1[r] ? (S = L ? *(unsigned short *) &mem[m ? P : 16 * r[Q ? p : 11] + (unsigned short) (r[6])] : mem[m ? P : 16 * r[Q ? p : 11] + (unsigned short) (r[6])], N = L ? *(unsigned short *) &mem[m ? P : 16 * r[Q ? p : 11] + (unsigned short) (r[6])] - (f = *(unsigned short *) &mem[16 * r[8] + (unsigned short) (r[7])]) : (mem[m ? P : 16 * r[Q ? p : 11] + (unsigned short) (r[6])] - (f = *(unsigned char *) &mem[16 * r[8] + (unsigned short) (r[7])]))), 43[u = 92, E] = !N, F(N > S), m || w(6), SP(!N == b) : 0), e-- || (o = L, (r[L = 4] += 2, (S = L ? *(unsigned short *) &M : M, N = L ? *(unsigned short *) &M = (f = *(unsigned short *) &mem[16 * r[10] + (unsigned short) (-2 + r[4])]) : (M = (f = *(unsigned char *) &mem[16 * r[10] + (unsigned short) (-2 + r[4])])))), m && (r[L = 4] += 2, (S = L ? *(unsigned short *) &9[r] : 9[r], N = L ? *(unsigned short *) &9[r] = (f = *(unsigned short *) &mem[16 * r[10] + (unsigned short) (-2 + r[4])]) : (9[r] = (f = *(unsigned char *) &mem[16 * r[10] + (unsigned short) (-2 + r[4])])))), m & 2 ? s((r[L = 4] += 2, (S = L ? *(unsigned short *) &V : V, N = L ? *(unsigned short *) &V = (f = *(unsigned short *) &mem[16 * r[10] + (unsigned short) (-2 + r[4])]) : (V = (f = *(unsigned char *) &mem[16 * r[10] + (unsigned short) (-2 + r[4])]))))) : o || (4[r] += c)), e-- || ((S = L ? *(unsigned short *) &U[mem] : U[mem], N = L ? *(unsigned short *) &U[mem] = (f = *(unsigned short *) &d) : (U[mem] = (f = *(unsigned char *) &d)))), e-- || (986[l] ^= 9, (S = L ? *(unsigned short *) &*E : *E, N = L ? *(unsigned short *) &*E = (f = *(unsigned short *) &l[m ? 2[r] : (char) c]) : (*E = (f = *(unsigned char *) &l[m ? 2[r] : (char) c])))), e-- || ((S = L ? *(unsigned short *) &l[m ? 2[r] : (char) c] : l[m ? 2[r] : (char) c], N = L ? *(unsigned short *) &l[m ? 2[r] : (char) c] = (f = *(unsigned short *) &*E) : (l[m ? 2[r] : (char) c] = (f = *(unsigned char *) &*E)))), e-- || (R = 2, b = L, Q && Q++), e-- || (W - U ? (S = L ? *(unsigned short *) &mem[W] : mem[W], N = L ? *(unsigned short *) &mem[W] ^= (f = *(unsigned short *) &mem[U]) : (mem[W] ^= (f = *(unsigned char *) &mem[U]))), (S = L ? *(unsigned short *) &mem[U] : mem[U], N = L ? *(unsigned short *) &mem[U] ^= (f = *(unsigned short *) &mem[W]) : (mem[U] ^= (f = *(unsigned char *) &mem[W]))), (S = L ? *(unsigned short *) &mem[W] : mem[W], N = L ? *(unsigned short *) &mem[W] ^= (f = *(unsigned short *) &mem[U]) : (mem[W] ^= (f = *(unsigned char *) &mem[U]))) : 0), e-- || ((S = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] : mem[16 * r[10] + (unsigned short) (--r[L = 4])], N = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned short *) &m[r]) : (mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned char *) &m[r])))), e-- || ((r[L = 4] += 2, (S = L ? *(unsigned short *) &m[r] : m[r], N = L ? *(unsigned short *) &m[r] = (f = *(unsigned short *) &mem[16 * r[10] + (unsigned short) (-2 + r[4])]) : (m[r] = (f = *(unsigned char *) &mem[16 * r[10] + (unsigned short) (-2 + r[4])]))))), e-- || (Q = 2, p = m, R && R++), e-- || (L = 0, O = *E, F(D(m += 3 * 42[E] + 6 * 40[E])), z(D(1 + m)), N = *E = D(m - 1)), e-- || (N = BP(m - 1)), e-- || (1[E] = -(1 & (L ? *(short *) &*E : *E) >> 8 * -~L - 1)), e-- || (2[r] = -(1 & (L ? *(short *) &*r : *r) >> 8 * -~L - 1)), e-- || (9[(S = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] : mem[16 * r[10] + (unsigned short) (--r[L = 4])], N = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned short *) &9[r]) : (mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned char *) &9[r]))), (S = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] : mem[16 * r[10] + (unsigned short) (--r[L = 4])], N = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned short *) &M + 5) : (mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned char *) &M + 5))), r] = BX, M = c), e-- || (J(), (S = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] : mem[16 * r[10] + (unsigned short) (--r[L = 4])], N = L ? *(unsigned short *) &mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned short *) &V) : (mem[16 * r[10] + (unsigned short) (--r[L = 4])] = (f = *(unsigned char *) &V)))), e-- || (s((r[L = 4] += 2, (S = L ? *(unsigned short *) &V : V, N = L ? *(unsigned short *) &V = (f = *(unsigned short *) &mem[16 * r[10] + (unsigned short) (-2 + r[4])]) : (V = (f = *(unsigned char *) &mem[16 * r[10] + (unsigned short) (-2 + r[4])])))))), e-- || (J(), s((V & ~m) + 1[E])), e-- || (J(), 1[E] = V), e-- || (L = o = 1, A = 4 * !T, O = t, W = h = T < 3 ? 16 * r[Q ? p : D(A + 3)] + (unsigned short) (D(A + 1)[r] + D(A + 2) * g + r[D(A)]) : K(t), U = V = K(a), o ? U = h, W = V : V, (S = L ? *(unsigned short *) &mem[W] : mem[W], N = L ? *(unsigned short *) &mem[W] = (f = *(unsigned short *) &mem[U]) : (mem[W] = (f = *(unsigned char *) &mem[U]))), (S = L ? *(unsigned short *) &mem[P + m] : mem[P + m], N = L ? *(unsigned short *) &mem[P + m] = (f = *(unsigned short *) &mem[h + 2]) : (mem[P + m] = (f = *(unsigned char *) &mem[h + 2])))), e-- || (++M, H(3)), e-- || (M += 2, H(c & m)), e-- || (++M, m[E] && H(4)), e-- || ((c &= m) ? 1[E] = *E / c, N = *E %= c : H(0)), e-- || (*r = N = m & E[L = 0] + c * 1[E]), e-- || (*E = -m[E]), e-- || (*E = mem[16 * r[Q ? p : m] + (unsigned short) (*E + r[3])]), e-- || (m[E] ^= 1), e-- || (E[m / 2] = m & 1), e-- || ((S = L ? *(unsigned short *) &*E : *E, N = L ? *(unsigned short *) &*E & (f = *(unsigned short *) &c) : (*E & (f = *(unsigned char *) &c)))), e-- || ((a = c), a-- || (write(1, E, 1)), a-- || (time(j + 3), memcpy(mem + 16 * r[8] + (unsigned short) (r[3]), localtime(j + 3), m)), a < 2 ? *E = ~lseek(O = 4[E][j], *(unsigned *) &5[r] << 9, 0) ? (a ? write : read) (O, mem + 16 * r[8] + (unsigned short) (r[3]), *r) : 0 : 0), O = u, D(16) ? v(0) : D(17) && G(F(0)), M += (T % 3 + 2 * !(!T * t - 6)) * D(20) + D(18) - D(19) * ~!!L, D(15) ? O = m = N, 41[43[44[E] = (1 & (L ? *(short *) &N : N) >> 8 * -~L - 1), E] = !N, E] = D(50) : 0, !++q ? kb = 1, *l ? SDL_PumpEvents(), k = k ? k : SDL_SetVideoMode(720, 348, 32, 0), DX() : k ? SDL_Quit(), k = 0 : 0 : 0;
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
