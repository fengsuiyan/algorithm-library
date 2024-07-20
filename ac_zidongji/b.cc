#include <bits/stdc++.h>

using namespace std;
using llint = long long;

typedef struct Node *trie;

const int N = 15, L = 25, SIG = 2, B = 998244353, LL = 110, M = 1 << 6;

int tot;
int n, l;
int len[N];
char s[N][L];
int a[N][L];
vector <int> split[N];
int msk[N][L];
int fl[N];
llint dp[LL][N * L][M];
int mx, MM;

void clear() {
  for (int i = 0; i < n * 2; ++i) split[i].clear();
  memset(dp, 0, sizeof(dp));
  memset(msk, 0, sizeof(msk));
}

void go(int a[], int b[], int n) {
  for (int i = n - 1, j = 0; i >= 0; --i, ++j) {
    b[j] = 1 - a[i];
  }
}

void add(llint &a, llint b) {
  a = (a + b) % B;
  if (a < 0) a += B;
}

struct Node {
  int match;
  trie pre, child[SIG];
} T[N * L];

void trie_init() {
  tot = 1;
  memset(T, 0, sizeof(T));
}

void insert(int *s, int n, int fl) {
  trie t = T;
  for (int i = 0; i < n; ++i) {
    int c = s[i];
    if (!t->child[c]) {
      t->child[c] = &T[tot++];
    }
    t = t->child[c];
  }
  t->match |= fl;
}

void build_graph() {
  trie t = T;
  queue <trie> Q;
  for (int i = 0; i < SIG; ++i) {
    if (t->child[i]) {
      t->child[i]->pre = t;
      Q.push(t->child[i]);
    } else {
      t->child[i] = t;
    }
  }
  while (!Q.empty()) {
    t = Q.front();
    Q.pop();
    t->match |= t->pre->match;
    for (int i = 0; i < SIG; ++i) {
      if (t->child[i]) {
        t->child[i]->pre = t->pre->child[i];
        Q.push(t->child[i]);
      } else {
        t->child[i] = t->pre->child[i];
      }
    }
  }
}

int get(int a[], int l, int n) {
  int ret = 0;
  for (int t = 0, j = l, k = l - 1; j < n || k >= 0; ++j, --k, ++t) {
    if (j < n) ret |= a[j] << t;
    else ret |= (1 - a[k]) << t;
  }
  return ret;
}

void init_sp() {
  for (int i = 0; i < n; ++i) {
    for (int sp = 1; sp < len[i]; ++sp) {
      bool ok = 1;
      for (int j = sp, k = sp - 1; j < len[i] && k >= 0; ++j, --k) {
        if (a[i][j] == a[i][k]) {
          ok = 0;
          break;
        }
      }
      if (ok) split[i].push_back(sp);
      msk[i][sp] = get(a[i], sp, len[i]);
    }
    /*
    cout <<  i << " : ";
    for (int sp : split[i]) cout << sp << "," << bitset<4>(msk[i][sp]) << " "; cout << endl;*/
  }
}

int check(int t, int i, int l) {
  for (int sp : split[i]) {
    if ((max(sp, len[i] - sp) <= l) && ((t & ((1 << max(sp, len[i] - sp)) - 1)) == msk[i][sp])) {
      return 1;
    }
  }
  return 0;
}

int get(int t, int p) {
  int ret = 0;
  for (int i = 0; i < n; ++i) {
    if (check(t, i, p))
      ret |= 1 << i;
  }
  //cout << "get " << bitset <4>(t) << " " << p << " " << ret << endl;
  return ret;
}

void dfs(int p, int amsk, int mm, int j) {
  //cout << "dfs " << p - 1 << " " << bitset <4>(amsk) << " " << bitset <6>(mm) << " " << j << endl;
  add(dp[p - 1][j][mm], 1);
  //cout << "hi " << p - 1 << " " << mm << " " << j << " " << dp[p - 1][mm][j] << endl;
  if (p == mx + 1) return;
  for (int i = 0; i < 2; ++i) {
    int t = amsk | (i << (p - 1));
    int mm2 = mm | get(t, p);
    //cout << "dfs " << p - 1 << " " << bitset <4>(t) << " " << bitset <6>(mm)<< " " << i << " " <<  mm2 << " " << j << endl;
    trie nw = T[j].child[i];
    int nj = nw - T;
    mm2 |= nw->match;
    dfs(p + 1, t, mm2, nj);
  }
}


void run() {
  int t;
  scanf("%d", &t);
  while (t--) {
    scanf("%d%d", &n, &l);
    clear();
    for (int i = 0; i < n; ++i) {
      scanf("%s", s[i]);
      len[i] = strlen(s[i]);
    }
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < len[i]; ++j)
        a[i][j] = s[i][j] - '0';
    }
    for (int i = 0; i < n; ++i) {
      go(a[i], a[i + n], len[i]);
      len[i + n] = len[i];
    }
    for (int i = 0; i < n; ++i) {
      fl[i] = 1 << i;
    }
    for (int i = 0; i < n; ++i) {
      fl[i + n] = 1 << i;
    }
    /*
    for (int i = 0; i < 2 * n; ++i) {
      cout << "len " << len[i] << endl;
      for (int j = 0; j < len[i]; ++j) cout << a[i][j] << " "; cout << endl;
    }*/
    init_sp();
    trie_init();
    for (int i = 0; i < 2 * n; ++i) {
      insert(a[i], len[i], fl[i]);
    }
    build_graph();
    mx = *max_element(len, len + n);
    MM = 1 << mx;
    dfs(1, 0, 0, 0);
    //dp[0][0][0] = 1;
    int MSK = 1 << n;
    /*
    cout << "ac: " << tot << " : ";
    for (int j = 0; j < tot; ++j) cout << j << "," << T[j].child[0] - T << "," << T[j].child[1] - T << "," << T[j].match << " ";*/
    for (int i = mx; i < l; ++i) {
      for (int j = 0; j < tot; ++j) {
        for (int msk = 0; msk < MSK; ++msk) if (dp[i][j][msk]) {
          //cout << i <<  "," << j << "," << msk << "," << dp[i][j][msk] << endl;
          for (int x = 0; x < 2; ++x) {
            trie t = T + j;
            //cout << t->child[x] << endl;
            int nj = t->child[x] - T;
            //cout << "hi " << i+1 << " " << nj << "," << T[nj].match << endl;
            int nmsk = msk | T[nj].match;
            add(dp[i + 1][nj][nmsk], dp[i][j][msk]);
          }
        }
      }
    }
    //cout << "hi " << dp[2][0][MSK-1] << endl;
    //dp[l][j][MSK-1]
    llint ans = 0;
    for (int j = 0; j < tot; ++j) {
      add(ans, dp[l][j][MSK - 1]);
    }
    printf("%lld\n", ans);
  }
}

int main() {
  run();
  return 0;
}
