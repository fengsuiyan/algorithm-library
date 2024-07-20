#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
typedef pair<int, int> PII;
const int MAXN = 100000 + 10;

struct Edge {
  int v, nx;
} E[MAXN << 1];

struct BIT;

struct Entry {
  int d, a;
  BIT *ds;
  Entry() {}
  Entry(int d, int a, BIT *p): d(d), a(a), ds(p) {}
};

struct BIT {
  vector<int> u;
  void init() {u.clear();}
  void add(int x) {u.push_back(x);}
  void build() {sort(u.begin(), u.end());}
  int get(int x) {return upper_bound(u.begin(), u.end(), x) - u.begin();}
} DS[MAXN << 6], *ds_cnt;

//0: le, 1: ge, 2: eq
vector<Entry> entrys[3][MAXN];
int G[MAXN], w[MAXN], n, m, q;

inline void addedge(int u, int v) {
  E[m].v = v; E[m].nx = G[u]; G[u] = m ++;
  E[m].v = u; E[m].nx = G[v]; G[v] = m ++;
}

namespace TreeSplit {
  int sz[MAXN], vs[MAXN], total, mins, rt;
  void init(int n) {
    for (int i = 0; i < n; ++ i) vs[i] = 0;
  }
  void getCenter(int u, int f = -1) {
    int mx = 0; sz[u] = 1;
    for (int it = G[u]; ~it; it = E[it].nx) {
      int v = E[it].v; if (v == f || vs[v]) continue;
      getCenter(v, u); sz[u] += sz[v];
      mx = max(mx, sz[v]);
    }
    mx = max(mx, total - sz[u]);
    if (mx < mins) mins = mx, rt = u;
  }
  void dfs(int u, int f, int d, int t, BIT *ins) {
    entrys[t][u].push_back(Entry(d, rt, ins));
    ds_cnt->add(d);
    for (int it = G[u]; ~it; it = E[it].nx) {
      int v = E[it].v; if (v == f || vs[v]) continue;
      bool ok = (t == 0 && w[v] <= w[u]) || (t == 1 && w[v] >= w[u]) || (t == 2 && w[v] == w[u]);
      if (ok) dfs(v, u, d + 1, t, ins);
    }
  }
  void build(int u, int tot) {
    total = tot; mins = tot * 2;
    getCenter(u);
    u = rt; vs[rt] = 1;
    //following code for calc new sz
    getCenter(u);
    for (int i = 0; i < 3; ++ i) {
      ds_cnt->init();
      BIT *p = ds_cnt;
      // rt->v le => v->rt ge
      if (i == 0) p = ds_cnt + 1;
      else if (i == 1) p = ds_cnt - 1;
      dfs(u, -1, 0, i, p);
      //d is set to 0
      ds_cnt->build(); ds_cnt ++;
    }
    rt = -1;
    for (int it = G[u]; ~it; it = E[it].nx) {
      int v = E[it].v; if (vs[v]) continue;

      ds_cnt->init();
      if (w[v] <= w[u]) dfs(v, u, 1, 0, ds_cnt + 1);
      // d is set to 1, because x->u->v, the edge u->v is the 1, for del, query x d, the vertex whose dis to rt is (d - rtd) ds->get(d-rtd), but should not count the vertex of the same subtree of x, so sub it
      //ds_cnt add because w[v]<=w[u] then for u w[u] >= w[v]
      //so ds_cnt+1
      //dfs doesn't add any thing to BIT *ins(ds_cnt+1), it just add ins to entry. and add d to ds_cnt.
      ds_cnt->build(); ds_cnt ++;

      ds_cnt->init();
      if (w[v] >= w[u]) dfs(v, u, 1, 1, ds_cnt - 1);
      ds_cnt->build(); ds_cnt ++;

      if (w[v] == w[u]) {
        ds_cnt->init();
        dfs(v, u, 1, 2, ds_cnt);
        ds_cnt->build();
        ds_cnt ++;
      }
    }
    for (int it = G[u]; ~it; it = E[it].nx) {
      int v = E[it].v; if (vs[v]) continue;
      build(v, sz[v]);
    }
  }
}

int solve(const vector<Entry> &e, int d) {
  int ret(0);
  for (auto &x: e) {
    int add = x.ds->get(d - x.d);
    if (x.a != -1) ret += add;
    else ret -= add;
    // d is set to 1, because x->u->v, the edge u->v is the 1, for del, query x d, the vertex whose dis to rt is (d - rtd) ds->get(d-rtd), but should not count the vertex of the same subtree of x, so sub it
  }
  return ret;
}

int main() {
  int T; scanf("%d", &T);
  for (int _ = 0; _ < T; ++ _) {
    scanf("%d%d", &n, &q); m = 0;
    for (int i = 0; i < n; ++ i) {
      scanf("%d", w + i);
      entrys[0][i].clear();
      entrys[1][i].clear();
      entrys[2][i].clear();
      G[i] = -1;
    }
    for (int i = 1; i < n; ++ i) {
      int u, v; scanf("%d%d", &u, &v);
      addedge(u - 1, v - 1);
    }
    ds_cnt = DS;
    TreeSplit::init(n);
    TreeSplit::build(0, n);
    for (int i = 0; i < q; ++ i) {
      int u, d; scanf("%d%d", &u, &d); -- u;
      int ret = solve(entrys[0][u], d) + solve(entrys[1][u], d) - solve(entrys[2][u], d);
      printf("%d\n", ret);
    }
  }
  return 0;
}
