#include <bits/stdc++.h>

using namespace std;

const int N = 1e5 + 10, SIG = 26;

struct Node {
  int ch[SIG], fa;
  //int sz;
  int len;
  Node() : len(0), fa(-1) {
    memset(ch, -1, sizeof(ch));
  }
} pool[(N << 1) + 5];
int last, root;

namespace SAM {

int cnt;

void init() {
  if (cnt)
    for (int i = 0; i < cnt; ++i)
      pool[i] = Node();
  cnt = 1;
  root = 0;
  last = root;
}

void add(int c) {
  int np = cnt++;
  //np->sz = 1;
  pool[np].len = pool[last].len + 1;
  int p = last;
  while (p != -1 && pool[p].ch[c] == -1) {
    pool[p].ch[c] = np;
    p = pool[p].fa;
  }
  if (p == -1) {
    pool[np].fa = root;
  } else {
    int q = pool[p].ch[c];
    if (pool[q].len != pool[p].len + 1) {
      int nq = cnt++;
      pool[nq].len = pool[p].len + 1;
      for (int i = 0; i < SIG; ++i) pool[nq].ch[i] = pool[q].ch[i];
      pool[nq].fa = pool[q].fa;
      //nq->sz should be 0 because q->fa = nq, in later code q->sz will be added to q->fa->sz i.e. nq->sz, so !!!!no nq->sz = q->sz !!!!
      //nq->sz = q->sz;
      pool[q].fa = pool[np].fa = nq;
      while (p != -1 && pool[p].ch[c] == q) {
	pool[p].ch[c] = nq;
	p = pool[p].fa;
      }
    } else {
      pool[np].fa = q;
    }
  }
  last = np;
}

int get(const char *s, int m) {
  int p = root;
  int ret = 0;
  for (int i = 0; i < m; ++i) {
    int c = s[i] - 'a';
    if (pool[p].ch[c] != -1) {
      p = pool[p].ch[c];
      ret++;
    } else {
      for (; p != -1 && pool[p].ch[c] == -1; p = pool[p].fa);
      if (p != -1) {
        p = root;
        ret = 0;
      } else {
        ret = pool[p].len + 1;
        p = pool[p].ch[c];
      }
    }
  }
  return ret;
}

}

char s[N * 3 + 10];
char *head[N];
int len[N];

void run() {
  int t;
  //cout << sizeof(pool) <<endl;
  scanf("%d", &t);
  while (t--) {
    int c;
    scanf("%d", &c);
    char *p = s;
    for (int i = 0; i < c; ++i) {
      scanf("%s", p);
      head[i] = p;
      len[i] = strlen(p);
      p[len[i]] = 0;
      p += len[i] + 1;
    }
    //for (int i = 0; i < c; ++i) puts(head[i]);
    int who = max_element(len, len + c) - len;
    SAM::init();
    p = head[who];
    int m = len[who];
    for (int i = 0; i < m; ++i) SAM::add(p[i] - 'a');
    bool ok = 1;
    for (int i = 0; i < c; ++i) {
      if (i == who) continue;
      if (SAM::get(head[i], len[i]) < len[i]) {
        ok = 0;
      }
    }
    if (ok) {
      puts(head[who]);
    } else {
      puts("No");
    }
  }
}

int main() {
  run();
  return 0;
}
