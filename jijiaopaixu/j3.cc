#include <bits/stdc++.h>

using namespace std;
using llint = long long;

const int N = 2010;

struct Point {
  llint x, y;
  bool upside() const {
    return y > 0 || y == 0 && x > 0;
  }
  bool operator==(const Point &a) const {
    return x == a.x && y == a.y;
  }
  Point operator+(const Point &a) const {
    return Point{x + a.x, y + a.y};
  }
  Point operator-(const Point &a) const {
    return Point{x - a.x, y - a.y};
  }
  llint operator*(const Point &a) const {
    return x * a.x + y * a.y;
  }
  llint operator%(const Point &a) const {
    return x * a.y - y * a.x;
  }
};

int n;
llint A, B;
Point pt[N];

bool operator<(const Point &l, const Point &r) {
  return l.upside() != r.upside() ? l.upside() : l % r > 0;
}

bool less_90(Point &a, Point &b) {
  if (a * b > 0 && a % b >= 0) return 1;
  return 0;
}

bool large_90(Point &a, Point &b) {
  if (a % b < 0) return 1;
  else {
    if (a * b < 0) return 1;
    return 0;
  }
}

bool equ_90(Point &a, Point &b) {
  if (a * b == 0 && a % b > 0) return 1;
  return 0;
}

bool same_dir(Point &a, Point &b) {
  if (a * b > 0 && a % b == 0) return 1;
  return 0;
}

llint go(vector <Point> &a, vector <Point> &b) {
  llint ret = 0;
  for (auto x : a) {
    for (auto y : b) {
      llint t = abs(x % y);
      if (t >= A && t <= B) ++ret;
    }
  }
  return ret;
}

int main() {
  scanf("%d%lld%lld", &n, &A, &B);
  A *= 2, B *= 2;
  for (int i = 1; i <= n; ++i) {
    int x, y;
    scanf("%d%d", &x, &y);
    pt[i].x = x;
    pt[i].y = y;
  }
  llint ans = 0;
  for (int i = 1; i <= n; ++i) {
    vector <Point> v;
    for (int j = 1; j <= n; ++j) if (j != i) {
      v.push_back(pt[j] - pt[i]);
    }
    sort(v.begin(), v.end());
    //for (auto p : v) cout << p.x << "," << p.y << " "; cout << endl;
    int z = v.size();
    for (int i = 0; i < v.size(); ++i) {
      if (v[i] < Point{0, 1} && i < z) {
        v.push_back(v[i]);
      }
    }
    //for (int i = 0; i < z; ++i) v.push_back(v[i]);
    vector < vector <Point> > ve;
    for (int i = 0; i < v.size(); ++i) {
      if (ve.empty() || (!ve.back().empty() && !same_dir(ve.back().back(), v[i]))) {
        ve.push_back(vector <Point>(1, v[i]));
      } else {
        ve.back().push_back(v[i]);
      }
    }
    /*
    for (auto &v : ve) {
      for (auto p : v) cout << p.x << "," << p.y << " "; cout << endl;
    }*/
    int p90 = 0;
    for (int i = 1; i < ve.size(); ++i) {
      Point nw = ve[i].back();
      while (p90 < i && large_90(ve[p90].back(), nw)) {
        ++p90;
      }
      //cout << p90 << endl;
      //cout << "hi " << ve[i].back().x << " " << ve[i].back().y << " " << ve[p90].back().x << " " << ve[p90].back().y << endl;
      if (equ_90(ve[p90].back(), nw)) {
        ans += go(ve[p90], ve[i]);
      }
    }
  }
  printf("%lld\n", ans);
  return 0;
}
