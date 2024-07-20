#include <bits/stdc++.h>

using namespace std;

const int N = 3e5 + 10;

int x[N], y[N], z[N];

struct Graph {
  int n;
  vector < pair <int, int> > e[N];
  void init(int _n) {
    n = _n;
    for (int i = 1; i <= n; ++i) e[i].clear();
  }
  void addedge(int u, int v, int z) {
    e[u].push_back({v, z});
  }
} g, h;

struct Tarjan {
  int low[N], dfn[N], stk[N], bl[N];
  int idx, top, block;
  bool vis[N], instk[N];
  void dfs(int u, int x, Graph &g) {
    vis[u] = 1;
    int v;
    low[u] = dfn[u] = ++idx;
    stk[top++] = u;
    instk[u] = 1;
    for (auto ee : g.e[u]) {
      v = ee.first;
      if (x == v) continue;
      if (!dfn[v]) {
	dfs(v, u, g);
	if (low[u] > low[v]) low[u] = low[v];
      } else if (instk[v] && low[u] > dfn[v]) {
	low[u] = dfn[v];
      }
    }
    if (low[u] == dfn[u]) {
      block++;
      do {
	v = stk[--top];
	instk[v] = 0;
	bl[v] = block;
      } while (v != u);
    }
  }
  void solve(Graph &g) {
    memset(dfn, 0, sizeof(dfn));
    memset(vis, 0, sizeof(vis));
    memset(instk, 0, sizeof(instk));
    idx = top = block = 0;
    for (int i = 1; i <= g.n; ++i)
      if (!vis[i]) dfs(i, -1, g);
  }
} t;

int ok[N], vis[N];

void dfs(int u) {
  vis[u] = 1;
  for (auto ee : h.e[u]) {
    int v = ee.first;
    if (vis[v]) continue;
    ok[v] |= ok[u] | ee.second;
  }
  for (auto ee : h.e[u]) {
    int v = ee.first;
    if (vis[v]) continue;
    dfs(v);
  }
}

int main() {
  int n, m;
  scanf("%d%d", &n, &m);
  g.init(n);
  for (int i = 1; i <= m; ++i)
    scanf("%d%d%d", x + i, y + i, z + i);
  for (int i = 1; i <= m; ++i) {
    g.addedge(x[i], y[i], z[i]);
    g.addedge(y[i], x[i], z[i]);
  }
  t.solve(g);
  h.init(t.block);
  for (int i = 1; i <= m; ++i) {
    if (t.bl[x[i]] == t.bl[y[i]]) {
      ok[t.bl[x[i]]] |= z[i];
    } else {
      h.addedge(t.bl[x[i]], t.bl[y[i]], z[i]);
      h.addedge(t.bl[y[i]], t.bl[x[i]], z[i]);
    }
  }
  memset(vis, 0, sizeof(vis));
  int a, b;
  scanf("%d%d", &a, &b);
  dfs(t.bl[a]);
  puts(ok[t.bl[b]] ? "YES" : "NO");
  return 0;
}
