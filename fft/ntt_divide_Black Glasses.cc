#include <bits/stdc++.h>

using namespace std;
using llint = long long;

const int N = 1e5 + 1, P = 998244353, g = 3;

llint a[N << 2], b[N << 2], c[N << 2];
llint fac[N] = {1}, f[N] = {1};

llint mypw(llint a, int b) {
  llint ret = 1;
  for (; b; b >>= 1, a = a * a % P)
    if (b & 1) ret = ret * a % P;
  return ret;
}


void ntt(llint *a, int n, bool inv = false) {
  llint w = 1, d = mypw(g, (P - 1) / n), t;
  int i, j, c, s;
  if (inv) {
    for (i = 1, j = n - 1; i < j; swap(a[i++], a[j--]));
    for (t = mypw(n, P - 2), i = 0; i < n; ++i) a[i] = a[i] * t % P;
  }
  for (s = n >> 1; s; s >>= w = 1, d = d * d % P)
    for (c = 0; c < s; ++c, w = w * d % P)
      for (i = c; i < n; i += s << 1) {
	a[i | s] = (a[i] + P - (t = a[i | s])) * w % P;
	a[i] = (a[i] + t) % P;
      }
  for (i = 1; i < n; ++i) {
    for (j = 0, s = i, c = n >> 1; c; c >>= 1, s >>= 1) j = j << 1 | s & 1;
    if (i < j) swap(a[i], a[j]);
  }
}

void solve(int l, int r) {
  if (l == r) {
    if (l == 0) f[l] = 0;
    else f[l] = (fac[l] - f[l] + P) % P;
    return;
  }
  int mid = (l + r) >> 1;
  solve(l, mid);
  int len = 2;
  for (; len <= r - l + 1; len <<= 1);
  len <<= 1;
  for (int i = 0; i < len; ++i) a[i] = b[i] = 0;
  for (int i = l; i <= mid; ++i) a[i - l] = f[i];
  for (int i = l; i <= r; ++i) b[i - l] = fac[i - l];
  ntt(a, len);
  ntt(b, len);
  for (int i = 0; i < len; ++i) a[i] = a[i] * b[i] % P;
  ntt(a, len, 1);
  for (int i = mid + 1; i <= r; ++i) f[i] = (f[i] + a[i - l]) % P;
  solve(mid + 1, r);
}

int main() {
  for (int i = 1; i < N; ++i) fac[i] = fac[i - 1] * i % P;
  /*
  for (int i = 1; i <= n; ++i) {
    for (int j = 0; j < i; ++j) 
      (f[i] += f[j] * fac[i - j] % P) %= P;
    f[i] = (fac[i] - f[i] + P) % P;
  }*/
  solve(0, N - 1);
  int t;
  scanf("%d", &t);
  while (t--) {
    int n;
    scanf("%d", &n);
    printf("%lld\n", f[n]);
  }
  return 0;
}
