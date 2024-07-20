#include <bits/stdc++.h>

using namespace std;

template <class T> void mini(T &l, T r) {l = min(l, r);}
template <class T> void maxi(T &l, T r) {l = max(l, r);}

string to_string(string s) {
  return '"' + s + '"';
}

string to_string(bool a) {
  return a ? "T" : "F";
}

string to_string(const char* s) {
  return to_string((string) s);
}

template <typename A, typename B>
string to_string(pair<A, B> p) {
  return "(" + to_string(p.first) + ", " + to_string(p.second) + ")";
}

template <typename A>
string to_string(A v) {
  bool first = true;
  string res = "{";
  for (const auto &x : v) {
    if (!first) {
      res += ", ";
    }
    first = false;
    res += to_string(x);
  }
  res += "}";
  return res;
}

void debug_out() { cerr << endl; }

template <typename Head, typename... Tail>
void debug_out(Head H, Tail... T) {
  cerr << " " << to_string(H);
  debug_out(T...);
}

#ifdef LOCAL
#define debug(...) cerr << "[" << #__VA_ARGS__ << "]:", debug_out(__VA_ARGS__)
#else
#define debug(...) 42
#endif

using ll = long long;
using ld = long double;

const int inf = 1e9 + 10, L = 10000, N = 510, M = 25;
const double MAG_DELTA = 20;

int T, P, Z;

struct Point {
  int x, y;
  Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
  Point operator+(const Point &r) const {
    return Point{x + r.x, y + r.y};
  }
  Point operator-() const {
    return Point{-x, -y};
  }
  Point operator-(const Point &r) const {
    return Point{x - r.x, y - r.y};
  }
  int operator*(const Point &r) const {
    return x * r.x + y * r.y;
  }
  bool ok() const {
    return -L <= x && x <= L && -L <= y && y <= L;
  }
  bool operator<(const Point &r) const {
    return make_pair(x, y) < make_pair(r.x, r.y);
  }
  bool operator==(const Point &r) const {
    return make_pair(x, y) == make_pair(r.x, r.y);
  }
  double abs() const {
    return sqrt((*this) * (*this));
  }
  Point operator*(int a) {
    return Point(x * a, y * a);
  }
} pt[M], pp[N], pz[M], oldpt[M];

string to_string(Point x) {
  return to_string(make_pair(x.x, x.y));
}

int dis(Point a, Point b) {
  return (a - b) * (a - b);
}

bool check(Point a, Point b) {
  return a * b * 1.0 / a.abs() / b.abs() > 0.65;
}

set <int> st_peo;
int contain_of_taxi[M];
int chased[N];
vector < pair <Point, vector <int> > > ans;
double ans_cost;
pair <double, vector < pair <Point, vector <int> > > > ans_mn;
Point d[] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

tuple <int, int, Point> get_mn_z(const Point &x) {
  int d = inf, who_i = -1;
  Point dir_i;
  for (int i = 0; i < Z; ++i) {
    int dd = dis(pz[i], x);
    if (dd < d) {
      d = dd;
      who_i = i;
      dir_i = pz[i] - x;
    }
  }
  return make_tuple(d, who_i, dir_i);
}

tuple <int, int, Point> get_mn_peo(int id, const Point &x, int p_tar = -1, bool chase_fl = 0) {
  int d = inf, who_i = -1;
  Point dir_i;
  for (int i : st_peo) if (i != p_tar && (!chase_fl || (chased[i] == -1 || chased[i] == id))) {
    int dd = dis(pp[i], x);
    if (dd < d) {
      d = dd;
      who_i = i;
      dir_i = pp[i] - x;
    }
  }
  if (who_i != -1)
    chased[who_i] = id;
  return make_tuple(d, who_i, dir_i);
}

void move(const Point &dir, const vector <int> &v) {
  ans_cost += dir.abs() * (1 + 1.0 * v.size() / T);
  debug(dir, v);
  for (int i = 0; i < T; ++i) debug(i, contain_of_taxi[i], pt[i]);
  ans.emplace_back(dir, v);
  for (int i : v) {
    pt[i] = pt[i] + dir;
  }
  debug("after");
  for (int i = 0; i < T; ++i) debug(i, contain_of_taxi[i], pt[i]);
  set <Point> tmp;
  for (int i = 0; i < T; ++i) {
    tmp.insert(pt[i]);
  }
  debug(tmp);
  assert(tmp.size() == T);
  int d, who_i;
  Point dir_i;
  for (int i : v) {
    if (contain_of_taxi[i] != -1) {
      tie(d, who_i, dir_i) = get_mn_z(pt[i]);
      if (d == 0) {
        contain_of_taxi[i] = -1;
      }
    } else {
      tie(d, who_i, dir_i) = get_mn_peo(i, pt[i], -1);
      if (d == 0) {
        contain_of_taxi[i] = who_i;
        st_peo.erase(who_i);
      }
    }
  }
  debug("end");
  for (int i = 0; i < T; ++i) debug(i, contain_of_taxi[i], pt[i]);
}

Point bfs(Point x, int id, const set <Point> &st) {
  priority_queue < pair <double, Point> > q;
  int dd, who_i;
  Point dir_i;

  set <Point> cant;
  for (auto i : st) cant.insert(i);
  for (int i = 0; i < T; ++i) {
    cant.insert(pt[i]);
  }

  if (contain_of_taxi[id] == -1) {
    tie(dd, who_i, dir_i) = get_mn_peo(id, pt[id]);
  } else {
    tie(dd, who_i, dir_i) = get_mn_z(pt[id]);
  }

  set <Point> vis;
  vis.emplace(x);
  q.emplace(make_pair(-dir_i.abs() * 0.0, x));
  while (!q.empty()) {
    auto t = q.top();
    q.pop();
    if (cant.find(t.second) == cant.end()) return t.second;
    for (int i = 0; i < 4; ++i) {
      auto t2 = t.second + d[i];
      if (t2.ok() && vis.find(t2) == vis.end()) {
        q.push(make_pair(- (t2 - x).abs() - (dir_i + x - t2).abs() * 0.0, t2));
        vis.insert(t2);
      }
    }
  }
  assert(0);
}

Point bfs2(Point x, int id, const set <Point> &st) {
  queue <Point> q;

  set <Point> cant;
  for (auto i : st) cant.insert(i);
  for (int i = 0; i < T; ++i) {
    cant.insert(pt[i]);
  }

  set <Point> vis;
  vis.emplace(x);
  q.emplace(x);
  while (!q.empty()) {
    auto t = q.front();
    q.pop();
    if (cant.find(t) == cant.end()) return t;
    for (int i = 0; i < 4; ++i) {
      auto t2 = t + d[i];
      if (t2.ok() && vis.find(t2) == vis.end()) {
        q.push(t2);
      }
    }
  }
  assert(0);
}

bool taxi_no_peo() {
  for (int i = 0; i < T; ++i) {
    debug(contain_of_taxi[i], i);
    if (contain_of_taxi[i] != -1) return 0;
  }
  return 1;
}

bool check(const Point &a, const Point &d, const vector <int> &no_use) {
  Point b = a + d;
  debug("check");
  debug(a, d, b);
  for (int i : no_use) {
    debug(i, pt[i]);
  }
  for (int i : no_use) {
    if (b == pt[i]) {
      return 0;
    }
  }
  return 1;
}

void run() {
  int start_time = clock();
  cin >> T;
  for (int i = 0; i < T; ++i) {
    cin >> pt[i].x >> pt[i].y;
  }
  for (int i = 0; i < T; ++i) {
    oldpt[i] = pt[i];
  }
  cin >> P;
  for (int i = 0; i < P; ++i) {
    cin >> pp[i].x >> pp[i].y;
  }
  cin >> Z;
  for (int i = 0; i < Z; ++i) {
    cin >> pz[i].x >> pz[i].y;
  }

  ans_mn.first = inf;
  vector <double> vMAG_2[2] = {{0.5, 1, 2}, {0.25, 0.5}};
  vector <int> vMAG = {1, 3, 4, 5, 7, 9, 32};

  //const int fl = 0;
  for (int fl = 0; fl < 2; ++fl) for (auto MAG : vMAG) for (auto MAG_2 : vMAG_2[MAG == 32]) {
    if ((float)(clock() - start_time) / CLOCKS_PER_SEC > 1.9) break;

    for (int i = 0; i < T; ++i) pt[i] = oldpt[i];
    fill_n(contain_of_taxi, T, -1);
    ans.clear();
    ans_cost = 0;
    for (int i = 0; i < P; ++i) st_peo.emplace(i);

    while (!(st_peo.empty() && taxi_no_peo())) {
      debug("hi");
      int mn = inf;
      pair <int, pair <int, int> > who;
      Point dir;
      int d, who_i;
      Point dir_i;
      for (int i = 0; i < T; ++i) {
        if (contain_of_taxi[i] == -1) {
          tie(d, who_i, dir_i) = get_mn_peo(i, pt[i]);
          if (mn > d) {
            mn = d;
            who = {0, {i, who_i}};
            dir = dir_i;
          }
        } else {
          tie(d, who_i, dir_i) = get_mn_z(pt[i]);
          if (mn > d) {
            mn = d;
            who = {1, {i, who_i}};
            dir = dir_i;
          }
        }
      }

      debug(dir, who, mn);

      int g = __gcd(abs(dir.x), abs(dir.y));
      Point dd = Point(dir.x / g, dir.y / g);

      debug(g);

      int l = 1;

      /*
      if (MAG == 16) {
        int t = g;
        for (int i = 1; i <= 20; ++i) {
          if (t == 0) break;
          int o = min(t, 1 << i);
          v_dir.emplace_back(dir.x / g * o, dir.y / g * o);
          t -= o;
        }
        reverse(v_dir.begin(), v_dir.end());
        } else {}*/
      if (g > MAG) {
        l = g / MAG + 1;
      }

      int left = g;

      while (left > 0) {
        int md = max(left / 3, 1);
        if (MAG == 32) l = rand() % md + 1;
        debug(l, MAG, left);
        for (int step = min(left, l); step <= left; ) {
          debug(step);

          dir = dd * step;
          //if (dir.abs() / T > MAG_DELTA) fl = 1;

          int p_tar = -1;
          for (int i : st_peo) {
            chased[i] = -1;
          }
          if (who.first == 0) {
            //st_peo.erase(who.second.second);
            p_tar = who.second.second;
            chased[p_tar] = who.second.first;
          }

          vector <int> v, not_use;
          v.emplace_back(who.second.first);

          for (int i = 0; i < T; ++i) if (i != who.second.first) {
            int mn1, mn2, who1, who2;
            Point dir1, dir2;
            if (contain_of_taxi[i] == -1) {
              tie(mn1, who1, dir1) = get_mn_peo(i, pt[i], p_tar, fl);
              tie(mn2, who2, dir2) = get_mn_peo(i, pt[i] + dir, p_tar, fl);
              //(check(dir1, dir) && who1 == who2 && mn1 > mn2 && (pt[i] + dir).ok())
              if (mn1 > mn2 && dir.abs() / T < (sqrt(mn1) - sqrt(mn2)) * (1 + 1.0 / T) && (pt[i] + dir).ok()) {
                v.emplace_back(i);
              }
            } else {
              tie(mn1, who1, dir1) = get_mn_z(pt[i]);
              tie(mn2, who2, dir2) = get_mn_z(pt[i] + dir);
              //(check(dir1, dir) && who1 == who2 && mn1 > mn2 && (pt[i] + dir).ok())
              if (mn1 > mn2 && dir.abs() / T < (sqrt(mn1) - sqrt(mn2)) * (1 + 1.0 / T) && (pt[i] + dir).ok()) {
                v.emplace_back(i);
              }
            }
          }

          for (int i = 0; i < T; ++i) {
            if (find(v.begin(), v.end(), i) == v.end()) {
              not_use.emplace_back(i);
            }
          }

          set <Point> st;
          for (int i : v) {
            st.emplace(pt[i] + dir);
          }
          //while (1) {
          //bool ok = 1;
          double cost_sum = 0;
          for (int i : not_use) {
            if (st.find(pt[i]) != st.end()) {
              //ok = 0;
              auto tmp = bfs(pt[i], i, st);
              auto cost_tmp = (tmp - pt[i]).abs() * (1 + 1.0 / T);
              double cost_mrg = inf;
              if ((pt[i] + dir).ok() && check(pt[i], dir, not_use)) {
                mini(cost_mrg, dir.abs() * 1.0 / T);
              }
              cost_sum += min(cost_mrg, cost_tmp);
              /*
                 if (cost_mrg < cost_tmp) {
                 v.emplace_back(i);
                 st.emplace(pt[i] + dir);
                 } else {
                 move(tmp - pt[i], {i});
                 }*/
            }
          }
          //if (ok && find(contain_of_taxi, contain_of_taxi + T, p_tar) == contain_of_taxi + T) break;
          //}
          if (cost_sum < MAG_2 || step == left) {
            for (int i : not_use) {
              if (st.find(pt[i]) != st.end()) {
                //ok = 0;
                auto tmp = bfs(pt[i], i, st);
                auto cost_tmp = (tmp - pt[i]).abs() * (1 + 1.0 / T);
                double cost_mrg = inf;
                if ((pt[i] + dir).ok() && check(pt[i], dir, not_use)) {
                  mini(cost_mrg, dir.abs() * 1.0 / T);
                }
                //cost_sum += min(cost_mrg, cost_tmp);
                if (cost_mrg < cost_tmp) {
                  v.emplace_back(i);
                  st.emplace(pt[i] + dir);
                } else {
                  move(tmp - pt[i], {i});
                }
              }
            }
            move(dir, v);
            left -= step;
            break;
          }

          debug(l, MAG, step);

          if (MAG == 32) ++step;
          else {
            if (step + l <= left) step += l;
            else step = left;
          }
        }
        debug(l, MAG, "ok");
      }
    }

    debug(ans_cost);

    if (ans_mn.first > ans_cost) {
      ans_mn = {ans_cost, ans};
    }
    debug(ans_mn.first);

  }
  assert(st_peo.empty() && taxi_no_peo());

  cout << ans_mn.second.size() << endl;
  for (auto p : ans_mn.second) {
    auto dir = p.first;
    auto v = p.second;
    cout << "MOVE " << dir.x << " " << dir.y << " ";
    cout << v.size();
    for (int i : v) {
      cout << " " << i + 1;
    }
    cout << endl;
  }
}

int main() {
  run();
  return 0;
}
