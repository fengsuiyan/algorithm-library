#include <bits/stdc++.h>

using namespace std;

const int N = 1010, L = 33, B = 7340033, root = 3;

int dp[L][N << 2];
// or maybe N << 3
// I think N << 2 is enough

int mypw(int x, int k) {
  int ret = 1;
  while (k) {
    if (k & 1) ret = 1ll * ret * x % B;
    k >>= 1;
    x = 1ll * x * x % B;
  }
  return ret;
}

void change(int y[], int l) {
  for (int i = 1, j = l / 2; i < l - 1; ++i) {
    if (i < j) swap(y[i], y[j]);
    int k = l / 2;
    while (j >= k) {
      j -= k;
      k >>= 1;
    }
    if (j < k) j += k;
  }
}

void fft(int y[], int l, int on) {
  change(y, l);
  for (int h = 2; h <= l; h <<= 1) {
    int wn = mypw(root, (B - 1) / h);
    if (on == -1) wn = mypw(wn, B - 2);
    for (int j = 0; j < l; j += h) {
      int w = 1;
      for (int k = j; k < j + h / 2; ++k) {
	int u = y[k];
	int t = 1ll * y[k + h / 2] * w % B;
	y[k] = u + t;
	if (y[k] >= B) y[k] -= B;
	y[k + h / 2] = u + B - t;
	if (y[k + h / 2] >= B) y[k + h / 2] -= B;
	w = 1ll * w * wn % B;
      }
    }
  }
  if (on == -1) {
    int inv = mypw(l, B - 2);
    for (int i = 0; i < l; ++i) {
      y[i] = 1ll * y[i] * inv % B;
    }
  }
}

void calc(int a[], int b[], int l) {
  l *= 2;
  for (int i = 0; i < l; ++i) {
    if (i < l / 2) a[i] = b[i];
    else a[i] = 0;
  }
  fft(a, l, 1);
  for (int i = 0; i < l; ++i) a[i] = 1ll * a[i] * a[i] % B;
  fft(a, l, -1);
}

void init() {
  dp[0][0] = 1;
  for (int i = 1; i < L; ++i) {
    calc(dp[i], dp[i - 1], 1 << 10);
    calc(dp[i], dp[i], 1 << 10);
    for (int j = (1 << 10) - 1; j > 0; --j) {
      dp[i][j] = dp[i][j - 1];
    }
    dp[i][0] = 1;
  }
}

int main() {
  int q;
  init();
  scanf("%d", &q);
  while (q--) {
    int n, k;
    scanf("%d%d", &n, &k);
    int cnt = 0;
    while (n > 1 && (n & 1)) {
      ++cnt;
      n >>= 1;
    }
    printf("%d\n", dp[cnt][k]);
  }
  return 0;
}
