#include <bits/stdc++.h>

using namespace std;

const int N = 2e5 + 10;

struct point {
  int x, y;
  int idx;
  point(int x = 0, int y = 0) : x(x), y(y) {}
  inline point operator-(const point &p) const {
    return point(x - p.x, y - p.y);
  }
  inline long long operator*(const point &p) const {
    return (long long)x * p.y - (long long)y * p.x;
  }
  inline bool operator<(const point &p) const {
    return x < p.x || (x == p.x && y < p.y);
  }
};

inline int convexHall(int n, point p[], point q[]) {
  if (n < 3) {
    memcpy(q, p, sizeof(point) * n);
    return n;
  }
  else {
    int c;
    sort(p, p + n);
    for (int i = c = 0; i < n; q[c++] = p[i++]) {
      for (; c > 1 && (q[c - 1] - q[c - 2]) * (p[i] - q[c - 1]) < 0; --c);
    }
    for (int i = n - 2, j = c; i >= 0; q[c++] = p[i--]) {
      for (; c > j && (q[c - 1] - q[c - 2]) * (p[i] - q[c - 1]) < 0; --c);
    }
    return c - 1;
  }
}

int n, m;
point p[N], ch[N];

int main() {
  scanf("%d", &n);
  for (int i = 0; i < n; ++i) {
    scanf("%d%d", &p[i].x, &p[i].y);
    p[i].idx = i;
  }
  scanf("%d", &m);
  for (int i = n; i < n + m; ++i) {
    scanf("%d%d", &p[i].x, &p[i].y);
    p[i].idx = i;
  }
  int N = convexHall(n + m, p, ch);
  bool res = 1;
  for (int i = 0; i < N && res; ++i) {
    if (ch[i].idx >= n)
      res = 0;
  }
  puts(res ? "YES" : "NO");
  return 0;
}
