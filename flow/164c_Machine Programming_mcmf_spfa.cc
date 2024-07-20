#include <bits/stdc++.h>

using namespace std;

const int M = 6e3 + 10, N = 2e3 + 10, inf = 1e9 + 10;

int n, k;
int s[N], t[N], c[N];

//max flow min cost
struct Network {
  typedef int VAL;
  VAL cost;
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
    //if (dis[T] < inf) return true; max flow
    if (dis[T] < 0) return true; // ke xing liu
    return false;
  }
  void change(int S, int T) {
    int nf = inf;
    for (int i = T; i != S; i = edge[fa[i] ^ 1].t) {
      nf = min(nf, edge[fa[i]].c);
    }
    cost += nf * dis[T];
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

int main() {
  scanf("%d%d", &n, &k);
  vector <int> v;
  for (int i = 0; i < n; ++i) {
    scanf("%d%d%d", s + i, t + i, c + i);
    t[i] += s[i];
    v.push_back(s[i]);
    v.push_back(t[i]);
  }
  sort(v.begin(), v.end());
  v.erase(unique(v.begin(), v.end()), v.end());
  net.clear(v.size() + 10);
  for (int i = 1; i < v.size(); ++i) {
    net.add_edge(i - 1, i, k, 0);
  }
  net.add_edge(v.size(), 0, k, 0);
  vector <int> a;
  for (int i = 0; i < n; ++i) {
    s[i] = lower_bound(v.begin(), v.end(), s[i]) - v.begin();
    t[i] = lower_bound(v.begin(), v.end(), t[i]) - v.begin();
    a.push_back(net.add_edge(s[i], t[i], 1, -c[i]));
  }
  net.mcmf(v.size(), v.size() - 1);
  for (auto i : a) {
    printf("%d ", 1 - net.edge[i].c);
  }
  return 0;
}
