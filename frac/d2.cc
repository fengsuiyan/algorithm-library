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

const int N = 1e5 + 10, B = 5;
const ld eps = 1e-12;

const ll inf = 1e18 + 10;

template<typename T>
struct fraction {
  T a, b;
  fraction(): a(0), b(1) {}
  fraction(T a, T b):a(a), b(b) {fix();}
  void fix() {
    T g = std::__gcd(abs(a), abs(b));
    a /= g, b /= g;
    if (b < 0) a = -a, b = -b;
  }
  bool operator == (const fraction &r) const {
    return a * r.b == b * r.a;
  }
  bool operator < (const fraction &r) const {
    return a * r.b < b * r.a;
  }
  bool operator <= (const fraction &r) const {
    return a * r.b <= b * r.a;
  }
  fraction operator - () const {
    return fraction{-a, b};
  }
  fraction operator + (const fraction &r) const {
    return fraction(a * r.b + b * r.a, b * r.b);
  }
  fraction operator + (const T x) const {
    return fraction(a + b * x, b);
  }
  fraction operator - (const fraction &r) const {
    return fraction(a * r.b - b * r.a, b * r.b);
  }
  fraction operator - (const T x) const {
    return fraction(a - b * x, b);
  }
  fraction operator * (const fraction &r) const {
    return fraction(a * r.a, b * r.b);
  }
  fraction operator * (const T x) const {
    return fraction(a * x, b);
  }
  fraction operator / (const fraction &r) const {
    return fraction(a * r.b, b * r.a);
  }
  fraction operator / (const T x) const {
    return fraction(a, b * x);
  }
  fraction& operator += (const fraction &r) {
    a = a * r.b + b * r.a;
    b *= r.b;
    fix();
    return *this;
  }
  fraction& operator += (const T x) {
    a += b * x;
    fix();
    return *this;
  }
  fraction& operator -= (const fraction &r) {
    a = a * r.b - b * r.a;
    b *= r.b;
    fix();
    return *this;
  }
  fraction& operator -= (const T x) {
    a -= b * x;
    fix();
    return *this;
  }
  fraction& operator *= (const fraction &r) {
    a *= r.a;
    b *= r.b;
    fix();
    return *this;
  }
  fraction& operator *= (const T x) {
    a *= x;
    fix();
    return *this;
  }
  fraction& operator /= (const fraction &r) {
    a *= r.b; b *= r.a;
    fix();
    return *this;
  }
  fraction& operator /= (const T x) {
    b *= x;
    fix();
    return *this;
  }
  // 如果乘法会爆long long, 使用下面两个函数比较大小
  static int cmp(T n1, T d1, T n2, T d2){//-1,0,1
    if (!(n1 && n2)) return n1 == n2 ? 0 : n1 < n2 ? -1 : 1;
    if (n1 > 0 ^ n2 > 0) return n1 < 0 ? -1 : 1;
    if (n1 < 0) return rc(d1, -n1, d2, -n2);
    return rc(d2, n2, d1, n1);
  }
  static int rc(T n1, T d1, T n2, T d2){
    T k1 = n1 / d1, k2 = n2 / d2;
    if (k1 != k2)return k1 < k2 ? -1 : 1;
    T r1 = n1 % d1, r2 = n2 % d2;
    if (r1 && r2) return rc(d2, r2, d1, r1);
    if (r1 && !r2) return 1;
    if (r2 && !r1) return -1;
    return 0;
  }
};

template<typename T>
string to_string(fraction<T> x) {
  string ret;
  ret += to_string(x.a);
  ret += "/";
  ret += to_string(x.b);
  return ret;
}

int n, w;
vector <int> x, v;
vector < fraction <ll> > t, y, a, b;

struct Bit {
  int u[N];
  void modify(int x, int v) {
    for (x += B; x < N; x += x & -x) {
      u[x] += v;
    }
  }

  int getsum(int x) {
    int s = 0;
    for (x += B; x; x -= x & -x) {
      s += u[x];
    }
    return s;
  }

  int query(int l, int r) {
    return getsum(r) - getsum(l - 1);
  }

} bit;

void run() {
  cin >> n >> w;
  x.resize(n);
  v.resize(n);
  y.resize(n);
  t.resize(n);
  a.resize(n);
  b.resize(n);
  for (int i = 0; i < n; ++i) {
    cin >> x[i];
    cin >> v[i];
  }

  for (int i = 0; i < n; ++i) {
    t[i] = fraction <ll>(v[i], x[i]);
    y[i] = fraction <ll>(1, x[i]);
  }

  debug(t, y);

  vector < fraction <ll> > va, vb;
  for (int i = 0; i < n; ++i) {
    a[i] = t[i] + y[i] * w;
    va.push_back(a[i]);
    b[i] = t[i] - y[i] * w;
    vb.push_back(b[i]);
  }
  
  sort(va.begin(), va.end());
  va.erase(unique(va.begin(), va.end()), va.end());
  sort(vb.begin(), vb.end());
  vb.erase(unique(vb.begin(), vb.end()), vb.end());

  int za = va.size(), zb = vb.size();

  debug(a, b);
  
  vector < pair <int, int> > vp;

  for (int i = 0; i < n; ++i) {
    int ta = lower_bound(va.begin(), va.end(), a[i]) - va.begin();
    int tb = lower_bound(vb.begin(), vb.end(), b[i]) - vb.begin(); 
    vp.emplace_back(ta, tb);
  }

  sort(vp.begin(), vp.end(), [](pair <int, int> a, pair <int, int> b){return make_pair(a.first, -a.second) < make_pair(b.first, -b.second);});

  debug(vp);
  ll ans = 0;
  for (int i = 0; i < n; ++i) {
    debug(i, bit.query(vp[i].second, zb), vp[i].second);
    ans += bit.query(vp[i].second, zb);
    bit.modify(vp[i].second, 1);
  }

  cout << ans << endl;
}

int main() {
  run();
  return 0;
}
