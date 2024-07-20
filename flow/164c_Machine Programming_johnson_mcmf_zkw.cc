#include <bits/stdc++.h>

using namespace std;

const int N = 2010;

struct Network {
  static const int N = 2010, M = 6e3 + 10;
  static const int INF = 1e9 + 7;
  typedef int VAL;
  typedef struct ARC {
    int t, c;
    VAL w;
    ARC *o;
  } *PTR;
  ARC edge[M];
  PTR now[N], e[N];
  int cnt[N], l[N], r[N], arc;
  VAL sum;
  #define REV(x) edge[((x) - edge) ^ 1]
  void clear() {
    sum = arc = 0;
  }
  int flow(int S, int T) {
    return johnson_flow(S, T);
  }
  PTR add_edge(int x, int y, int c, VAL w = 0) {
    e[x] = &(edge[arc++] = (ARC){y, c, +w, e[x]});
    e[y] = &(edge[arc++] = (ARC){x, 0, -w, e[y]});
    return e[x];
  }
  int aug(int S, int T, int &can) {
    int x, z = T, use = can;
    for (x = S; x != T; x = now[x]->t) if (use > now[x]->c) use = now[z = x]->c;
    for (x = S; x != T; x = now[x]->t) {
      now[x]->c -= use;
      REV(now[x]).c += use;
      sum += use * now[x]->w;
    }
    can -= use;
    return z;
  }
  int johnson_flow(int S, int T, int can = INF) {
    int in = can, x, m;
    VAL phi[N], len[N], maxw = INF;
    fill_n(phi, N, maxw);
    phi[r[m = 0] = T] = 0;
    for (int i = 0; i <= m; ++i) {
      l[x = r[i % N]] = 0;
      for (PTR u = e[x]; u; u = u->o) {
	if (phi[x] + REV(u).w >= phi[u->t] || !REV(u).c) continue;
	phi[u->t] = phi[x] + REV(u).w;
	if (!l[u->t]) l[r[++m % N] = u->t] = 1;
      }
    }
    do {
      typedef pair <VAL, int> TPL;
      priority_queue <TPL> q;
      fill_n(len, N, maxw);
      memset(l, 0, sizeof(l));
      for (q.push(TPL(len[T] = 0, T)); q.size(); l[x] = 1) {
	x = q.top().second;
	q.pop();
	if (!l[x]) for (PTR u = e[x]; u; u = u->o) if (REV(u).c) {
	  VAL at = len[x] + phi[x] + REV(u).w - phi[u->t];
	  if (at >= len[u->t]) continue;
	  len[u->t] = at;
	  now[u->t] = &REV(u);
	  q.push(TPL(-at, u->t));
	}
      }
      for (x = 0; x < N; ++x) phi[x] += len[x];
    } while (len[S] < maxw && aug(S, T, can) != T);
    return in - can;
  }
} net;

int locate(const vector <int> &u, int x) {
  return lower_bound(u.begin(), u.end(), x) - u.begin();
}

int main() {
  int n, k, s[N], t[N], c[N];
  scanf("%d%d", &n, &k);
  vector <int> u;
  for (int i = 0; i < n; ++i) {
    scanf("%d%d%d", s + i, t + i, c + i);
    t[i] += s[i];
    u.push_back(s[i]);
    u.push_back(t[i]);
  }
  net.clear();
  sort(u.begin(), u.end());
  u.erase(unique(u.begin(), u.end()), u.end());
  for (int i = 0; i < (int)u.size() - 1; ++i) {
    net.add_edge(i, i + 1, k, 0);
  }
  net.add_edge(u.size(), 0, k, 0);
  vector <Network::PTR> a;
  for (int i = 0; i < n; ++i) {
    s[i] = locate(u, s[i]);
    t[i] = locate(u, t[i]);
    a.push_back(net.add_edge(s[i], t[i], 1, -c[i]));
  }
  net.flow(u.size(), u.size() - 1);
  for (auto x : a) printf("%d ", 1 - x->c);
  return 0;
}
