#include <bits/stdc++.h>

using namespace std;

#define mid ((l + r) >> 1)
#define lson (p << 1)
#define rson (p << 1 | 1)

const int N = 1.4e4 + 10, M = 3e4 + 10, H = N / 2, L = N * 5;

struct Edge {
  int x, y, w;
  bool operator<(const Edge &a) const {
    return w < a.w;
  }
};

int n, m, tot, nwtime;
int x[M], y[M], w[M];
vector<Edge> v;
int bg[L], ed[L], szs[L], mxe[L];
double ans;
int fa[N], sz[N], id[N];

void init() {
  for (int i = 1; i <= n; ++i) {
    fa[i] = i;
    sz[i] = 1;
  }
}

int find(int x) {
  return x == fa[x] ? x : fa[x] = find(fa[x]);
}

void mrg(int x, int y, int nwt) {
  x = find(x), y = find(y);
  if (x != y) {
    ed[id[x]] = nwt - 1;
    ed[id[y]] = nwt - 1;
    int ii = ++tot;
    fa[x] = y;
    sz[y] += sz[x];
    id[x] = 0;
    id[y] = ii;
    szs[id[y]] = sz[y];
    bg[id[y]] = nwt;
  }
}

vector<int> tr[L * 4];

void update(int p, int l, int r, int ll, int rr, int x) {
  if (ll > rr) return;
  if (ll > r || rr < l) return;
  if (ll <= l && rr >= r) {
    tr[p].push_back(x);
    return;
  }
  update(lson, l, mid, ll, rr, x);
  update(rson, mid + 1, r, ll, rr, x);
}

int get(const bitset<H> &msk) {
  for (int i = n / 2; i >= 0; --i) if (msk[i]) return i;
  return 0;
}

void dfs(int p, int l, int r, bitset<H> msk) {
  for (int i : tr[p]) {
    msk |= msk << szs[i];
  }
  if (l == r) {
    if (get(msk) == 0) return;
    ans = min(ans, (double)mxe[l] / get(msk));
    return;
  }
  dfs(lson, l, mid, msk);
  dfs(rson, mid + 1, r, msk);
}

int main() {
  scanf("%d%d", &n, &m);
  for (int i = 0; i < m; ++i) {
    scanf("%d%d%d", x + i, y + i, w + i);
    x[i]++, y[i]++;
    v.push_back(Edge{x[i], y[i], w[i]});
  }
  sort(v.begin(), v.end());
  init();
  for (int i = 1; i <= n; ++i) {
    int ii = ++tot;
    id[i] = ii;
    szs[id[i]] = 1;
  }
  reverse(v.begin(), v.end());
  nwtime = 0;
  vector<int> vid;
  memset(ed, -1, sizeof(ed));
  for (int i = 0; i < m; ++i) {
    if (i == 0 || v[i].w != v[i - 1].w) {
      for (int j : vid) {
        mrg(v[j].x, v[j].y, nwtime);
      }
      mxe[nwtime] = v[i].w;
      nwtime++;
      vid.clear();
    }
    vid.push_back(i);
  }
  /*
  for (int i = 1; i <= tot; ++i) {
    printf("%d: %d %d %d\n", i, bg[i], ed[i], szs[i]);
  }*/
  for (int i = 1; i <= tot; ++i) if (ed[i] == -1) ed[i] = nwtime - 1;
  for (int i = 1; i <= tot; ++i) {
    update(1, 0, nwtime - 1, bg[i], ed[i], i);
  }
  //for (int i = 0; i <= nwtime; ++i) printf("%d ", mxe[i]); puts("");
  ans = 1e18;
  dfs(1, 0, nwtime - 1, 1);
  printf("%.9f\n", ans);
  return 0;
}
