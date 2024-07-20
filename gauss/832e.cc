#include <bits/stdc++.h>

using namespace std;
using llint = long long;

const int N = 510, B = 1e9 + 7;

int n, m;
int e[N][2 * N];
int x[N];
int ps[N];
char s[N][N];
char bs[N];
int inv[] = {0, 1, 3, 2, 4};
int pw[N] = {1};
int rk;

int solve(int n, int m, int ms) {
  int r = 0;
  for (int c = 0, i, j, k; c < m && r < n; ++c) {
    for (i = k = r; i < n; ++i) {
      if (e[i][c] > e[k][c]) k = i;
      if (k != r) for (j = c; j < ms; ++j) swap(e[k][j], e[r][j]);
      if (e[r][c] == 0) {
      } else {
	for (i = r + 1; i < n; ++i) {
	  int t = e[i][c] * inv[e[r][c]] % 5;
	  for (int j = c; j < ms; ++j) e[i][j] = (e[i][j] - t * e[r][j] + 100) % 5;
	}
	++r;
      }
    }
  }
  return r;
}

bool check(int x) {
  for (int j = rk; j < m; ++j) {
    if (e[j][x + n] != 0)
      return 0;
  }
  return 1;
}

void init() {
  for (int i = 1; i < N; ++i) pw[i] = pw[i - 1] * 5ll % B;
}

int main() {
  init();
  scanf("%d%d", &n, &m);
  for (int i = 0; i < n; ++i) {
    scanf("%s", s[i]);
  }
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      e[j][i] = s[i][j] - 'a';
    }
  }
  //x[i]
  //sig(e[j][i] * x[i][q]) = b[q][i]
  int q;
  scanf("%d", &q);
  for (int i = 0; i < q; ++i) {
    scanf("%s", bs);
    for (int j = 0; j < m; ++j) {
      e[j][i + n] = bs[j] - 'a';
    }
  }
  rk = solve(m, n, n + q);
  for (int i = 0; i < q; ++i) {
    if (check(i)) {
      printf("%d\n", pw[n - rk]);
    } else {
      puts("0");
    }
  }
  return 0;
}
