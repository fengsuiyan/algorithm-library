#include <bits/stdc++.h>

using namespace std;
using llint = long long;

const int N = 1010, Mod = 1e9 + 7;

int q, a, b;
llint fac[N], inv[N];
int f[N][N][2];

llint updadd(llint &x, llint y) {
  x += y;
  x %= Mod;
  if (x < 0) x += Mod;
  return x;
}

llint mypw(llint a, llint b) {
  llint ret = 1;
  while (b) {
    if (b & 1) ret = ret * a % Mod;
    a = a * a % Mod;
    b >>= 1;
  }
  return ret;
}

void init() {
  fac[0] = 1;
  for (int i = 1; i < N; ++i) {
    fac[i] = fac[i - 1] * i % Mod;
  }
  for (int i = 0; i < N; ++i) {
    inv[i] = mypw(fac[i], Mod - 2);
  }
}

llint C(llint a, llint b) {
  if (a < b) return 0;
  return fac[a] * inv[b] % Mod * inv[a - b] % Mod;
}

vector <int> go() {
  vector <int> v;
  for (int n = 1; n <= 500; ++n) {
    llint ans = 0;
    for (llint i = 0; i <= n; ++i) {
      llint t = mypw(i, a) * mypw(n - i, b) % Mod;
      updadd(ans, C(i + 1, n - i) * t % Mod);
    }
    v.push_back(ans);
  }
  return v;
}

vector <int> BM(vector <int> &s) {
  vector <int> C(1, 1), B(1, 1);
  int L = 0, m = 1, b = 1;
  for (int n = 0; n < s.size(); ++n) {
    llint d = 0;
    for (int i = 0; i <= L; ++i) d = (d + (llint)C[i] * s[n - i]) % Mod;
    if (d == 0) ++m;
    else if (2 * L <= n) {
      vector <int> T = C;
      llint c = Mod - d * mypw(b, Mod - 2) % Mod;
      while (C.size() < B.size() + m) C.push_back(0);
      for (int i = 0; i < B.size(); ++i) C[i + m] = (C[i + m] + c * B[i]) % Mod;
      L = n + 1 - L;
      B = T;
      b = d;
      m = 1;
    } else {
      llint c = Mod - d * mypw(b, Mod - 2) % Mod;
      while (C.size() < B.size() + m) C.push_back(0);
      for (int i = 0; i < B.size(); ++i) C[i + m] = (C[i + m] + c * B[i]) % Mod;
      ++m;
    }
  }
  return C;
}

namespace cayleyhamilton {
const int N = 10010;
llint res[N], base[N], _c[N], _md[N];

vector <int> Md;

void mul(llint *a, llint *b, int k) {
  for (int i = 0; i < k + k; ++i) _c[i] = 0;
  for (int i = 0; i < k; ++i) if (a[i])
    for (int j = 0; j < k; ++j)
      _c[i + j] = (_c[i + j] + a[i] * b[j]) % Mod;
  for (int i = k + k - 1; i >= k; --i) if (_c[i])
    for (int j = 0; j < Md.size(); ++j)
      _c[i - k + Md[j]] = (_c[i - k + Md[j]] - _c[i] * _md[Md[j]]) % Mod;
  for (int i = 0; i < k; ++i) a[i] = _c[i];
}

llint solve(llint n, vector <int> &a, const vector <int> &b) {
  llint ans = 0, pnt = 0;
  int k = a.size();
  assert(a.size() == b.size());
  for (int i = 0; i < k; ++i) _md[k - 1 - i] = -a[i];
  _md[k] = 1;
  Md.clear();
  for (int i = 0; i < k; ++i) if (_md[i] != 0) Md.push_back(i);
  for (int i = 0; i < k; ++i) res[i] = base[i] = 0;
  res[0] = 1;
  while ((1ll << pnt) <= n) pnt++;
  for (int p = pnt; p >= 0; --p) {
    mul(res, res, k);
    if ((n >> p) & 1) {
      for (int i = k - 1; i >= 0; --i) res[i + 1] = res[i];
      res[0] = 0;
      for (int j = 0; j < Md.size(); ++j) res[Md[j]] = (res[Md[j]] - res[k] * _md[Md[j]]) % Mod;
    }
  }
  for (int i = 0; i < k; ++i) ans = (ans + res[i] * b[i]) % Mod;
  if (ans < 0) ans += Mod;
  return ans;
}
}

int main() {
  init();
  int t;
  scanf("%d", &t);
  while (t--) {
    scanf("%d%d%d", &q, &a, &b);
    vector <int> t = go();
    vector <int> c = BM(t);
    c.erase(c.begin());
    for (int i = 0; i < c.size(); ++i) {
      c[i] = (Mod - c[i]) % Mod;
    }
    while (q--) {
      llint n;
      scanf("%lld", &n);
      printf("%lld\n", cayleyhamilton::solve(n - 1, c, vector <int>(t.begin(), t.begin() + c.size())));
    }
  }
  return 0;
}
