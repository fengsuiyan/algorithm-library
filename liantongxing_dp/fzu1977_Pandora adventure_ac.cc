#include <cstdio>
#include <vector>
#include <cstring>

using namespace std;

const int N = 15, L = 3;

typedef long long ll;

int n, m;
int mz[N][N];
bool isend;

struct hashmp {
  static const int B = 1e4 + 7, M = 1e5 + 10;
  int head[B], nxt[M], sz;
  ll s[M], f[M];
  void clear() {
    sz = 0;
    memset(head, -1, sizeof(head));
  }
  ll ins(long long a, long long b) {
    int h = a % B;
    for (int p = head[h]; ~p; p = nxt[p]) if (s[p] == a) {
      return f[p] += b;
    }
    s[sz] = a;
    f[sz] = b;
    nxt[sz] = head[h];
    head[h] = sz++;
    return b;
  }
} f[2];

void tovec(ll s, int *a) {
  for (int i = m; i >= 0; --i) a[i] = s & ((1 << L) - 1), s >>= L;
  isend = s & 1;
}

ll toll(int *a) {
  int vis[N];
  memset(vis, 0, sizeof(vis));
  int k = 0;
  for (int i = 0; i <= m; ++i) if (a[i]) a[i] = vis[a[i]] ? vis[a[i]] : vis[a[i]] = ++k;
  ll s = isend;
  for (int i = 0; i <= m; ++i) s = s << L | a[i];
  return s;
}

void shift(int *a) {
  for (int i = m; i > 0; --i) a[i] = a[i - 1];
  a[0] = 0;
}

inline ll tollnxt(int *a, int j) {
  if (j == m) shift(a);
  // wa code: if (j == m) toll(a) >> L;
  return toll(a);
}

void print(int *a) {
  printf("vec: ");
  for (int i = 0; i <= m; ++i) printf("%d ", a[i]); puts("");
}

void dpblank(int i, int j, int pre) {
  int a[N];
  //puts("blank");
  for (int k = 0; k < f[pre].sz; ++k) {
    tovec(f[pre].s[k], a);
    //printf("%d %d\n", i, j);
    //print(a);
    int x = a[j - 1], y = a[j];
    if (isend) {
      if (x || y || mz[i][j] == 2) continue;
      a[j - 1] = a[j] = 0;
      f[pre ^ 1].ins(tollnxt(a, j), f[pre].f[k]);
    }
    else {
      if (x && y) {
        if (x == y) {
          a[j - 1] = a[j] = 0;
          isend = 1;
          f[pre ^ 1].ins(tollnxt(a, j), f[pre].f[k]);
        }
        else {
          a[j - 1] = a[j] = 0;
          for (int t = 0; t <= m; ++t)
            if (a[t] == y) a[t] = x;
          f[pre ^ 1].ins(tollnxt(a, j), f[pre].f[k]);
        }
      }
      else if ((x && !y) || (!x && y)) {
        if (mz[i][j + 1]) {
          a[j - 1] = 0;
          a[j] = x + y;
          f[pre ^ 1].ins(tollnxt(a, j), f[pre].f[k]);
        }
        if (mz[i + 1][j]) {
          a[j - 1] = x + y;
          a[j] = 0;
          f[pre ^ 1].ins(tollnxt(a, j), f[pre].f[k]);
        }
      }
      else {
        if (mz[i][j + 1] && mz[i + 1][j]) {
          a[j - 1] = a[j] = 13;
          f[pre ^ 1].ins(tollnxt(a, j), f[pre].f[k]);
        }
        if (mz[i][j] == 1) {
          a[j - 1] = a[j] = 0;
          f[pre ^ 1].ins(tollnxt(a, j), f[pre].f[k]);
        }
      }
    }
  }
}

void dpblock(int i, int j, int pre) {
  int a[N];
  for (int k = 0; k < f[pre].sz; ++k) {
    tovec(f[pre].s[k], a);
    a[j - 1] = a[j] = 0;
    f[pre ^ 1].ins(tollnxt(a, j), f[pre].f[k]);
  }
}

ll solve() {
  int pre = 0;
  f[pre].clear();
  f[pre].ins(0, 1);
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= m; ++j) {
      f[pre ^ 1].clear();
      if (mz[i][j]) dpblank(i, j, pre);
      else dpblock(i, j, pre);
      pre ^= 1;
    }
  }
  ll ans = 0;
  for (int i = 0; i < f[pre].sz; ++i)
    ans += f[pre].f[i];
  return ans;
}

int main() {
  int t;
  scanf("%d", &t);
  char s[N + 10];
  for (int _ = 1; _ <= t; ++_) {
    scanf("%d%d", &n, &m);
    memset(mz, 0, sizeof(mz));
    for (int i = 1; i <= n; ++i) {
      scanf("%s", s + 1);
      for (int j = 1; j <= m; ++j) {
        if (s[j] == '*') mz[i][j] = 1;
        else if (s[j] == 'O') mz[i][j] = 2;
      }
    }
    printf("Case %d: %I64d\n", _, solve());
  }
  return 0;
}
