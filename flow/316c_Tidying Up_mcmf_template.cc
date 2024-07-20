#include <bits/stdc++.h>

using namespace std;

const int NN = 85, N = NN * NN, M = N << 3, inf = 1e9 + 10;

int n, m;
int a[NN][NN];
int di[] = {1, 0};
int dj[] = {0, 1};

bool in(int i, int j) {
  return i >= 0 && i < n && j >= 0 && j < m;
}

struct Network {
  typedef int VAL;
  VAL cost;
  int flow;
  int n, m;
  struct Edge {
    int t, c;
    VAL w;
    int nxt;
  } edge[M];
  int e[N];
  VAL dis[N];
  int fa[N];
  int in[N];
  void clear(int _n) {
    n = _n;
    m = 0;
    cost = 0;
    fill_n(e, n, -1);
  }
  int add_edge(int x, int y, int c, VAL w) {
    edge[m] = Edge{y, c, w, e[x]};
    e[x] = m++;
    edge[m] = Edge{x, 0, -w, e[y]}; 
    e[y] = m++;
    return e[x];
  }
  bool spfa(int S, int T) {
    fill_n(dis, n, inf);
    memset(in, 0, sizeof(in));
    queue <int> q;
    q.push(S);
    dis[S] = 0;
    in[S] = 1;
    while (!q.empty()) {
      int t = q.front();
      q.pop();
      in[t] = 0;
      for (int j = e[t]; ~j; j = edge[j].nxt) {
	int v = edge[j].t;
	if (edge[j].c > 0 && dis[v] > dis[t] + edge[j].w) {
	  dis[v] = dis[t] + edge[j].w;
	  fa[v] = j;
	  if (!in[v]) {
	    in[v] = 1;
	    q.push(v);
	  }
	}
      }
    }
    if (dis[T] < inf) return true; // max flow
    // if (dis[T] < 0) return true; kexing liu
    return false;
  }
  void change(int S, int T) {
    int nf = inf;
    for (int i = T; i != S; i = edge[fa[i] ^ 1].t) {
      nf = min(nf, edge[fa[i]].c);
    }
    cost += nf * dis[T];
    flow += nf;
    for (int i = T; i != S; i = edge[fa[i] ^ 1].t) {
      edge[fa[i]].c -= nf;
      edge[fa[i] ^ 1].c += nf;
    }
  }
  VAL mcmf(int S, int T) {
    cost = 0;
    while (spfa(S, T)) change(S, T);
    return cost;
  }
} net;

int ha(int i, int j) {
  return i * m + j;
}

void add(int i, int j, int ni, int nj) {
  if ((i + j) % 2) swap(i, ni), swap(j, nj);
  net.add_edge(ha(i, j), ha(ni, nj), 1, a[i][j] == a[ni][nj] ? 0 : 1);
}

int main() {
  scanf("%d%d", &n, &m);
  net.clear(n * m + 10);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      scanf("%d", a[i] + j);
    }
  }
  int S = n * m, T = n * m + 1;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      for (int k = 0; k < 2; ++k) {
	int ni = i + di[k];
	int nj = j + dj[k];
	if (in(ni, nj)) {
	  add(i, j, ni, nj);
	}
      }
    }
  }
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      if ((i + j) % 2)
	net.add_edge(ha(i, j), T, 1, 0);
      else
	net.add_edge(S, ha(i, j), 1, 0);
    }
  }
  printf("%d\n", net.mcmf(S, T));
  return 0;
}
