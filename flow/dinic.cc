// fast enough dinic standard code and the code to AC hdu2444
#include <bits/stdc++.h>

using namespace std;

const int N = 210;
const int INF = 1e5 + 10;

int n, m;
bool ff[N];
bool vis[N];
int a[M], b[M];
vector<int> g[N];
bool dd;

struct Dinic {
  struct edge {
    int to, c;
  };
  vector<edge> e;
  int src, sink;
  vector<int> g[N];
  int dis[N];
  int cur[N];
  void init(int s, int t) {
    src = s, sink = t;
    clear();
  }
  void clear() {
    e.clear();
    for (int i = 0; i < N; ++i) {
      g[i].clear();
    }
    memset(dis, -1, sizeof(dis));
    memset(cur, 0, sizeof(cur));
  }
  void add(int u, int v, int w) {
    e.push_back(edge{v, w});
    g[u].push_back(e.size() - 1);
    e.push_back(edge{u, 0});
    g[v].push_back(e.size() - 1);
  }
  bool bfs() {
    memset(dis, -1, sizeof(dis));
    queue<int> q;
    q.push(src);
    dis[src] = 0;
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (int i = 0; i < g[v].size(); ++i) {
        int u = e[g[v][i]].to;
        int w = e[g[v][i]].c;
        if (w > 0 && dis[u] == -1) {
          q.push(u);
          dis[u] = dis[v] + 1;
        }
      }
    }
    if (dis[sink] == -1) return 0;
    return 1;
  }
  int dfs(int v, int mn) {
    if (v == sink || mn == 0) return mn;
    for (int &i = cur[v], tmp; i < g[v].size(); ++i) {
      int u = e[g[v][i]].to;
      int w = e[g[v][i]].c;
      if (w > 0 && dis[u] == dis[v] + 1 && (tmp = dfs(u, min(mn, w)))) {
        e[g[v][i]].c -= tmp;
        e[g[v][i] ^ 1].c += tmp;
        return tmp;
      }
    }
    return 0;
  }
  int dinic() {
    int tmp = 0, res = 0;
    while (bfs()) {
      memset(cur, 0, sizeof(cur));
      while (tmp = dfs(src, INF)) res += tmp;
    }
    return res;
  }
} dinic;

void dfs(int v, int f) {
  ff[v] = f;
  vis[v] = 1;
  for (int i = 0; i < g[v].size(); ++i) {
    int u = g[v][i];
    if (!vis[u]) {
      dfs(u, f ^ 1);
    }
    else {
      if (ff[u] == f)
        dd = 0;
    }
  }
}


int main() {
  while (scanf("%d%d", &n, &m) > 0) {
    for (int i = 0; i < N; ++i) {
      g[i].clear();
    }
    for (int i = 0; i < m; ++i) {
      scanf("%d%d", a + i, b + i);
      g[a[i]].push_back(b[i]);
      g[b[i]].push_back(a[i]);
    }
    dd = 1;
    memset(vis, 0, sizeof(vis));
    for (int i = 1; i <= n && dd; ++i) {
      if (!vis[i])
        dfs(i, 0);
    }
    if (dd) {
      int sc = 0, sk = n + 1;
      dinic.init(0, n + 1);
      for (int i = 1; i <= n; ++i) {
        if (ff[i])
          dinic.add(sc, i, 1);
        else
          dinic.add(i, sk, 1);
      }
      for (int i = 0; i < m; ++i) {
        if (ff[a[i]])
          dinic.add(a[i], b[i], 1);
        else
          dinic.add(b[i], a[i], 1);
      }
      printf("%d\n", dinic.dinic());
    }
    else {
      puts("No");
    }
  }
  return 0;
}
