#include <bits/stdc++.h>

using namespace std;
using llint = long long;

const int N = 1e5 + 10, SIG = 27;

vector <int> e[N << 1];
char s[N];

struct Node {
  Node *ch[SIG], *fa;
  int sz;
  int len;
  Node() : len(0), fa(NULL), sz(0) {
    memset(ch, 0, sizeof(ch));
  }
} pool[(N << 1) + 5], *last, *root;

void add_edge(int x, int y) {
  e[x].push_back(y);
}

namespace SAM {

int cnt;

void init() {
  if (cnt)
    for (int i = 0; i < cnt; ++i)
      pool[i] = Node();
  cnt = 1;
  root = &pool[0];
  last = root;
}

void add(int c) {
  Node *np = &pool[cnt++];
  np->sz = 1;
  np->len = last->len + 1;
  Node *p = last;
  while (p && !p->ch[c]) {
    p->ch[c] = np;
    p = p->fa;
  }
  if (!p) {
    np->fa = root;
  } else {
    Node *q = p->ch[c];
    if (q->len != p->len + 1) {
      Node *nq = &pool[cnt++];
      nq->len = p->len + 1;
      for (int i = 0; i < SIG; ++i) nq->ch[i] = q->ch[i];
      nq->fa = q->fa;
      //nq->sz should be 0 because q->fa = nq, in later code q->sz will be added to q->fa->sz i.e. nq->sz, so no nq->sz = q->sz
      //nq->sz = q->sz;
      q->fa = np->fa = nq;
      while (p && p->ch[c] == q) {
	p->ch[c] = nq;
	p = p->fa;
      }
    } else {
      np->fa = q;
    }
  }
  last = np;
}

void add_fa_tree() {
  for (int i = 1; i < cnt; ++i) {
    assert(pool[i].fa - pool != i);
    add_edge(pool[i].fa - pool, i);
  }
}

void dfs(int v, int f) {
  for (int u : e[v]) {
    if (u == f) continue;
    dfs(u, v);
    pool[v].sz += pool[u].sz;
  }
}

}

int main() {
  SAM::init();
  scanf("%s", s);
  int m = strlen(s);
  for (int i = 0; i < m; ++i) SAM::add(s[i] - 'a');
  /*
  for (int i = 1; i < SAM::cnt; ++i) {
    cout << "hi " << pool[i].fa->len << " " << pool[i].len << endl;
    for (int j = pool[i].fa->len + 1; j <= pool[i].len; ++j) {
      printf("%d ", pool[i].sz);
    }
    puts("");
  }*/
  SAM::add_fa_tree();
  SAM::dfs(0, 0);
  /*
  for (int i = 1; i < SAM::cnt; ++i) {
    cout << "hi " << pool[i].fa->len << " " << pool[i].len << endl;
    for (int j = pool[i].fa->len + 1; j <= pool[i].len; ++j) {
      printf("%d ", pool[i].sz);
    }
    puts("");
  }*/
  llint ans = 0;
  for (int i = 1; i < SAM::cnt; ++i) {
    ans += 1ll * (pool[i].sz + 1) * pool[i].sz / 2 * (pool[i].len - pool[i].fa->len);
  }
  printf("%lld\n", ans);
  return 0;
}
