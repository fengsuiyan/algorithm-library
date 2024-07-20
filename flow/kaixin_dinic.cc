#include <bits/stdc++.h>

using namespace std;

const int N = 1e4 + 10, SN = 110, inf = 1e9 + 10;

int n, m;
char s[SN][SN];
int di[] = {0, 1, 0, -1};
int dj[] = {-1, 0, 1, 0};

int ha(int x, int y) {
  return x * m + y;
}

struct Dinic {
  struct Edge {
    int to, c;
  };
  int n;
  vector <Edge> e;
  vector <int> g[N];
  int dis[N];
  int cur[N];
  void init(int _n) {
    n = _n;
    clear(n);
  }
  void clear(int n) {
    e.clear();
    for (int i = 0; i < n; ++i) g[i].clear();
    fill_n(dis, n, -1);
    fill_n(cur, n, 0);
  }
  void add(int u, int v, int w) {
    e.push_back(Edge{v, w});
    g[u].push_back(e.size() - 1);
    e.push_back(Edge{u, 0});
    g[v].push_back(e.size() - 1);
  }
  bool bfs(int sc, int sk) {
    fill_n(dis, n, -1);
    queue <int> q;
    q.push(sc);
    dis[sc] = 0;
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
    if (dis[sk] == -1) return 0;
    return 1;
  }
  int dfs(int v, int mn, int sk) {
    if (v == sk || mn == 0) return mn;
    for (int &i = cur[v], tmp; i < g[v].size(); ++i) {
      int u = e[g[v][i]].to;
      int w = e[g[v][i]].c;
      if (w > 0 && dis[u] == dis[v] + 1 && (tmp = dfs(u, min(mn, w), sk))) {
	e[g[v][i]].c -= tmp;
	e[g[v][i] ^ 1].c += tmp;
	return tmp;
      }
    }
    return 0;
  }
  int dinic(int sc, int sk) {
    int tmp = 0, ret = 0;
    while (bfs(sc, sk)) {
      fill_n(cur, n, 0);
      while (tmp = dfs(sc, inf, sk)) ret += tmp;
    }
    return ret;
  }
} dinic;

bool in(int x, int y) {
  return x >= 0 && x < n && y >= 0 && y < m;
}

int main() {
  int t;
  scanf("%d", &t);
  while (t--) {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n; ++i) {
      scanf("%s", s[i]);
    }
    dinic.init(n * m + 10);
    int sc = n * m, sk = n * m + 1;
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
	for (int k = 0; k < 4; ++k) {
	  int ni = i + di[k], nj = j + dj[k];
	  if (in(ni, nj) && s[i][j] == s[ni][nj]) {
	    dinic.add(ha(i, j), ha(ni, nj), 1);
	  } else {
	    if (k & 1)
	      dinic.add(ha(i, j), sk, 1);
	    else
	      dinic.add(sc, ha(i, j), 1);
	  }
	}
      }
    }
    printf("%d\n", dinic.dinic(sc, sk) / 2);
  }
  return 0;
}
