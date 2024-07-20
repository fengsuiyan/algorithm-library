#include <bits/stdc++.h>

using namespace std;

const int N = 1e5 + 10, L = 20, seed = 133331;
const long long F = 284517164;
//const long long F = 354871524;

map <int, vector <int> > mp;
int n, a[N];
long long p[L];
long long h[N][L];

void doit() {
  p[0] = seed;
  for (int i = 0; i < L - 1; ++i) {
    p[i + 1] = p[i] * p[i];
  }
  for (int j = 0; j < L; ++j) {
    for (int i = 0; i < n; ++i) {
      if (i + (1 << j) <= n) {
	long long &tmp = h[i][j];
	if (!j) tmp = a[i] ^ F;
	else tmp = (h[i][j - 1]) * p[j - 1] + h[i + (1 << (j - 1))][j - 1];
      }
    }
  }
}

int lcp(int a, int b) {
  if (a > b) swap(a, b);
  int ret = 0;
  for (int i = L - 1; i >= 0; --i) {
    if (b + (1 << i) > n) continue;
    if (h[a][i] == h[b][i]) {
      a += (1 << i);
      b += (1 << i);
      ret += (1 << i);
    }
  }
  return ret;
}

bool check(int a, int b) {
  int s = b - a;
  return lcp(a, b) >= s;
}

struct Seg {
  int l, r, s;
  Seg(int _l, int _r) : l(_l), r(_r), s(r - l) {}
  bool operator<(const Seg &o) const {
    if (s != o.s) return s < o.s;
    return l < o.l;
  }
};

int main() {
  scanf("%d", &n);
  for (int i = 0; i < n; ++i) {
    scanf("%d", a + i);
    mp[a[i]].push_back(i);
  }
  doit();
  vector <Seg> v;
  for (auto &kv : mp) {
    auto &tmp = kv.second;
    for (int i = 0; i < (int)tmp.size(); ++i) {
      for (int j = i + 1; j < (int)tmp.size(); ++j) {
	if (check(tmp[i], tmp[j]))
	 v.push_back(Seg(tmp[i], tmp[j] - 1));
      }
    }
  }
  sort(v.begin(), v.end());
  int now = 0;
  for (int i = 0; i < (int)v.size(); ++i) {
    auto &tmp = v[i];
    if (tmp.l >= now) now = tmp.r + 1;
  }
  printf("%d\n", n - now);
  for (int i = now; i < n; ++i) printf("%d%c", a[i], " \n"[i == n - 1]);
  return 0;
}
