#include <bits/stdc++.h>

using namespace std;
using namespace placeholders;

template <class T> void mini(T &l, T r) {l = min(l, r);}
template <class T> void maxi(T &l, T r) {l = max(l, r);}

template <class TH> void _dbg(const char *sdbg, TH h) {
  cerr << sdbg << "=" << h << "\n";
}
template <class TH, class ...TA> void _dbg(const char *sdbg, TH h, TA ...a) {
  while (*sdbg != ',') cerr << *sdbg++; cerr << "=" << h << ","; _dbg(sdbg + 1, a...);
}
template <class T> ostream &operator<<(ostream &os, vector <T> v) {
  os << "[";
  for (auto vv : v) os << vv << ",";
  return os << "]";
}
template <class L, class R> ostream &operator<<(ostream &os, pair <L, R> p) {
  return os << "(" << p.first << "," << p.second << ")";
}

#define eput(...) _dbg(#__VA_ARGS__, __VA_ARGS__)

using llint = long long;

const int N = 2e5 + 10;

set <int> *merge(set <int> *a, set <int> *b) {
  if (a->size() < b->size()) swap(a, b);
  a->insert((*b).begin(), (*b).end());
  delete b;
  return a;
}

struct Node {
  Node *go[27], *par;
  int val, id;
  vector <Node *> ch;
  Node() {
    memset(go, 0, sizeof(go));
    val = 0;
    par = 0;
    id = -1;
  }
  int n_include;
  
  set <int> *dfs() {
    set <int> *st = new set <int>();
    if (id != -1) st->insert(id);
    for (auto it = ch.begin(); it != ch.end(); ++it) {
      st = merge(st, (*it)->dfs());
    }
    n_include = st->size();
    return st;
  }

};

Node pool[N * 2 + 10], *cur = pool;
Node *root, *last;

void init() {
  root = last = cur++;
}

void extend(int w, int id = -1) {
  Node *p = last, *np = cur++;
  np->val = p->val + 1;
  np->id = id;
  while (p && p->go[w] == 0) p->go[w] = np, p = p->par;
  if (p == 0) {
    np->par = root;
  } else {
    Node *q = p->go[w];
    if (q->val == p->val + 1)
      np->par = q;
    else {
      Node *nq = cur++;
      memcpy(nq->go, q->go, sizeof(q->go));
      nq->val = p->val + 1;
      nq->par = q->par;
      q->par = np->par = nq;
      while (p && p->go[w] == q)
        p->go[w] = nq, p = p->par;
    }
  }
  last = np;
}

string a[N];
int n, k;

void run() {
  init();
  cin >> n >> k;
  for (int i = 0; i < n; ++i) {
    cin >> a[i];
    for (int j = 0; j < a[i].size(); ++j) {
      extend(a[i][j] - 'a', i);
    }
    extend(26);
  }

  for (Node *i = pool; i != cur; ++i) {
    if (i->par)
      i->par->ch.push_back(i);
  }

  root->dfs();

  for (int i = 0; i < n; ++i) {
    string &s = a[i];
    int l = 0;
    Node *c = root;
    llint ans = 0;
    for (int j = 0; j < s.size(); ++j) {
      int w = s[j] - 'a';
      while (c && c->go[w] == 0) {
        c = c->par;
        if (c)
          l = c->val;
      }
      if (c) {
        c = c->go[w];
        ++l;
      } else {
        c = root;
        l = 0;
      }
      while (c && c->n_include < k) {
        c = c->par;
        if (c) l = c->val;
        else l = 0;
      }
      ans += l;
    }
    cout << ans << " ";
  }

  cout << endl;
}

int main() {
  run();
  return 0;
}
