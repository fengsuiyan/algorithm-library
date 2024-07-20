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

#define eput(...) //_dbg(#__VA_ARGS__, __VA_ARGS__)

using llint = long long;

const llint BASE = 1ll << 32;

struct Triple {
  llint c, s, x;
};

const Triple ZERO = {0, 0, 0};

struct Node {
  llint l, r, m, c, s, x;
  Node *_pl, *_pr;
  
  Node(llint l, llint r) :
    l(l), r(r), m((l + r) / 2), c(0), s(0), x(0), _pl(nullptr), _pr(nullptr) {
  }

  ~Node() {
    delete _pl;
    delete _pr;
  }

  bool is_leaf() const {
    return r == l;
  }

  void add(llint i, llint d) {
    c += d;
    s += d * i;
    if (!is_leaf()) {
      if (i <= m) {
        if (_pl == nullptr) _pl = new Node(l, m);
        _pl->add(i, d);
      } else {
        if (_pr == nullptr) _pr = new Node(m + 1, r);
        _pr->add(i, d);
      }
      x = (_pl == nullptr ? 0 : _pl->x) + (_pr == nullptr ? 0 : _pr->x);
      if (_pl != nullptr && _pr != nullptr) {
        x += _pl->c * _pr->s - _pr->c * _pl->s;
      }
    }
  }

  Triple query(llint l, llint r) {
    if (l > r || c == 0 || l > this->r || r < this->l) {
      return ZERO;
    } else if (l <= this->l && r >= this->r) {
      eput(this->l, this->r, c, s, x);
      return {c, s, x};
    } else {
      Triple tl = _pl == nullptr ? ZERO : _pl->query(l, r);
      Triple tr = _pr == nullptr ? ZERO : _pr->query(l, r);
      eput(tl.c, tl.s, tl.x);
      eput(tr.c, tr.s, tr.x);
      return {tl.c + tr.c, tl.s + tr.s, tl.x + tr.x + tl.c * tr.s - tr.c * tl.s};
    }
  }

};

int n, m;
llint t, l, r;
vector <llint> x;

void run() {
  scanf("%d", &n);
  Node *p = new Node(0, BASE + BASE);
  for (int i = 0; i < n; ++i) {
    scanf("%lld", &t);
    t += BASE;
    p->add(t, 1);
    x.push_back(t);
    eput(p->x, p->s, p->c);
  }
  eput(x);

  scanf("%d", &m);
  for (int i = 0; i < m; ++i) {
    scanf("%lld%lld%lld", &t, &l, &r);
    if (t == 1) {
      --l;
      p->add(x[l], -1);
      x[l] += r;
      p->add(x[l], 1);
    } else {
      l += BASE;
      r += BASE;
      auto t = p->query(l, r);
      eput(t.c, t.s, t.x);
      printf("%lld\n", p->query(l, r).x);
    }
  }

  delete p;

}

int main() {
  run();
  return 0;
}
