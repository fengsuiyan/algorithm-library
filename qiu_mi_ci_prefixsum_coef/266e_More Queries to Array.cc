#include <bits/stdc++.h>

using namespace std;
using llint = long long;

#define mid ((l + r) >> 1)
#define lson (p << 1)
#define rson (p << 1 | 1)

const int N = 1e5 + 10, K = 6, B = 1e9 + 7;

int n, m;
int a[N];
llint C[K + 1][K + 1];
int coef[K + 1][K + 1];
llint dd[N][K];

struct Node {
  llint sum[K];
  int lz;
} tr[N * 4];

llint mypw(llint a, llint b) {
  llint ret = 1;
  while (b) {
    if (b & 1) ret = ret * a % B;
    a = a * a % B;
    b >>= 1;
  }
  return ret;
}

llint add(llint x, llint y) {
  x = (x + y);
  if (x > B) x -= B;
  if (x < 0) x += B;
  return x;
}

void init() {
  C[0][0] = 1;
  for (int i = 1; i <= K; ++i) {
    C[i][0] = 1;
    for (int j = 1; j <= i; ++j) {
      C[i][j] = add(C[i - 1][j], C[i - 1][j - 1]);
    }
  }
  coef[0][0] = 0;
  coef[0][1] = 1;
  for (int i = 1; i < K; ++i) {
    vector <int> vl(K + 1, 0);
    for (int j = i + 1; j >= 0; --j) {
      vl[j] = C[i + 1][j];
    }
    //(x + 1) ^ (i + 1) = C[i + 1][1] * sigma(x ^ i) + C[i + 1][2] * sigma(x ^ (i - 1)
    vector <int> vr(K + 1, 0);
    for (int j = 0; j < i; ++j) {
      for (int k = 0; k <= K; ++k) {
        vr[k] = add(vr[k], coef[j][k] * C[i + 1][j] % B);
      }
    }
    vr[0] = add(vr[0], 1);
    for (int k = 0; k <= K; ++k) {
      vl[k] = (vl[k] - vr[k] + B) % B;
    }
    //cout << i << ": ";
    //for (int k = 0; k <= K; ++k) cout << vl[k] << " "; cout << "/" << C[i + 1][1] << endl;
    for (int k = 0; k <= K; ++k) {
      coef[i][k] = vl[k] * mypw(C[i + 1][1], B - 2) % B;
    }
  }
  for (int i = 1; i <= n; ++i) {
    llint pw = 1;
    for (int k = 0; k < K; ++k) {
      dd[i][k] = add(dd[i - 1][k], pw);
      pw = pw * i % B;
    }
  }
}

llint calc(int x, int k) {
  return dd[x][k];
  if (x == 0) return 0;
  llint p = 1;
  llint ret = 0;
  for (int i = 0; i <= k + 1; ++i) {
    ret = add(ret, coef[k][i] * p % B);
    p = p * x % B;
  }
  if (ret < 0) ret += B;
  return ret;
}

void update_node(int p, int l, int r, int v) {
  tr[p].lz = v;
  int len = r - l + 1;
  for (int i = 0; i < K; ++i) {
    tr[p].sum[i] = add(calc(r, i), (B - calc(l - 1, i))) * v % B;
  }
}

llint calc(int p, int l, int r, int ll, int k) {
  llint ret = 0;
  int t = 1 - ll;
  if (t == 0) {
    return tr[p].sum[k];
  }
  llint tt = 1;
  for (int i = 0; i <= k; ++i) {
    ret = add(ret, C[k][i] * tr[p].sum[k - i] % B * tt % B);
    tt = tt * t % B;
  }
  return ret;
}

void push_down(int p, int l, int r) {
  if (tr[p].lz >= 0) {
    update_node(lson, l, mid, tr[p].lz);
    update_node(rson, mid + 1, r, tr[p].lz); 
    tr[p].lz = -1;
  }
}

void push_up(int p, int l, int r) {
  for (int i = 0; i < K; ++i) {
    tr[p].sum[i] = add(tr[lson].sum[i], tr[rson].sum[i]);
  }
}

void build(int p, int l, int r) {
  if (l == r) {
    llint pw = 1;
    for (int i = 0; i < K; ++i) {
      tr[p].sum[i] = a[l] * pw % B;
      pw = pw * l % B;
    }
    return;
  }
  tr[p].lz = -1;
  build(lson, l, mid);
  build(rson, mid + 1, r);
  push_up(p, l, r);
}

void update(int p, int l, int r, int ll, int rr, int v) {
  if (ll <= l && rr >= r) {
    update_node(p, l, r, v);
    return;
  }
  push_down(p, l, r);
  if (ll <= mid)
    update(lson, l, mid, ll, rr, v);
  if (rr >= mid + 1)
    update(rson, mid + 1, r, ll, rr, v);
  push_up(p, l, r);
}

llint query(int p, int l, int r, int ll, int rr, int k) {
  if (ll <= l && rr >= r) {
    return calc(p, l, r, ll, k);
  }
  llint ret = 0;
  push_down(p, l, r);
  if (ll <= mid) 
    ret = add(ret, query(lson, l, mid, ll, rr, k));
  if (rr >= mid + 1)
    ret = add(ret, query(rson, mid + 1, r, ll, rr, k));
  return ret;
}

int main() {
  scanf("%d%d", &n, &m);
  init();
  for (int i = 1; i <= n; ++i) {
    scanf("%d", a + i);
  }
  build(1, 1, n);
  /*
  for (int i = 1; i <= n; ++i) {
    update(1, 1, n, i, i, a[i]);
  }*/
  for (int i = 1; i <= m; ++i) {
    char cmd[10];
    int l, r, x;
    scanf("%s%d%d%d", cmd, &l, &r, &x);
    if (cmd[0] == '=') {
      update(1, 1, n, l, r, x);
    } else {
      printf("%lld\n", query(1, 1, n, l, r, x));
    }
  }
  return 0;
}
