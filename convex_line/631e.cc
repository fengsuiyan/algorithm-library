#include <bits/stdc++.h>

using namespace std;
using llint = long long;

const int N = 2e5 + 10;

struct Line {
  llint a, b;
  llint get(llint x) {
    return a * x + b;
  }
};

struct Convex {
  vector <Line> hull;

  void clear() {
    hull.clear();
  }

  bool is_bad(const Line &p, const Line &c, const Line &n) {
    return (c.b - n.b) * (n.a - p.a) <= (p.b - n.b) * (n.a - c.a);
  }

  void add_line(llint a, llint b) {
    while (hull.size() >= 2 && is_bad(hull[hull.size() - 2], hull[hull.size() - 1], Line{a, b})) {
      hull.pop_back();
    }
    hull.push_back(Line{a, b});
  }

  llint query(llint x) {
    int l = -1, r = (int)hull.size() - 1;
    while (l + 1 < r) {
      int m = (l + r) / 2;
      if (hull[m].get(x) <= hull[m + 1].get(x))
        l = m;
      else
        r = m;
    }
    return hull[r].get(x);
  }

} convex;

int n, a[N];
llint sum[N];
llint ans, dans;

void run() {
  scanf("%d", &n);
  for (int i = 1; i <= n; ++i) scanf("%d", a + i);
  for (int i = 1; i <= n; ++i) {
    sum[i] = sum[i - 1] + a[i];
  }
  convex.clear();
  for (int j = 2; j <= n; ++j) {
    convex.add_line(j - 1, -sum[j - 2]);
    dans = max(dans, convex.query(a[j]) + sum[j - 1] - a[j] * (llint)j);
  }
  convex.clear();
  for (int i = n - 1; i >= 1; --i) {
    // in fact is convex.add(i + 1, -sum[i + 1]), but for the template k must be increase, so change to -(i + 1), -sum[i + 1], then query change to (-a[i]), in fact is j * a[i] - sum[j], i.e. (-j) * (-a[i]) - sum[j]
    convex.add_line(-(i + 1), -sum[i + 1]);
    dans = max(dans, convex.query(-a[i]) + sum[i] - a[i] * (llint)i);
  }
  for (int i = 1; i <= n; ++i) dans += a[i] * (llint)i;
  printf("%lld\n", dans);
}

int main() {
  run();
  return 0;
}
