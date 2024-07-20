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

const int N = 2e5 + 10, B = 1e9 + 7;
ll mypw(ll a, int b) {
  ll ret = 1;
  while (b) {
    if (b & 1) ret = ret * a % B;
    b >>= 1;
    a = a * a % B;
  }
  return ret;
}
 
struct Modint {
  ll x;
 
  Modint(ll a = 0) {
    x = a % B;
    if (x < 0) x += B;
  }
 
  Modint inv() {
    return Modint(mypw(x, B - 2));
  }

  Modint operator+=(Modint b);
  Modint operator-=(Modint b);
  Modint operator*=(Modint b);
  Modint operator/=(Modint b);
  
};

Modint operator+(Modint a, Modint b) {
  return Modint(a.x + b.x);
}
 
Modint operator-(Modint a, Modint b) {
  return Modint(a.x - b.x);
}
 
Modint operator*(Modint a, Modint b) {
  return Modint(a.x * b.x);
}
 
Modint operator/(Modint a, Modint b) {
  return a * b.inv();
}

Modint Modint::operator+=(Modint b) {
  *this = *this + b;
  return *this;
}

Modint Modint::operator-=(Modint b) {
  *this = *this - b;
  return *this;
}

Modint Modint::operator*=(Modint b) {
  *this = *this * b;
  return *this;
}

Modint Modint::operator/=(Modint b) {
  *this = *this / b;
  return *this;
}

string to_string(Modint a) {
  return to_string(a.x);
}

array <Modint, 3> go_dp(array <Modint, 3> &a, array <Modint, 3> &b) {
  array <Modint, 3> c = {};
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      c[min(2, i + j)] += a[i] * b[j];
    }
  }
  return c;
}

void run() {
  int n;
  cin >> n;
  vector <int> p(n + 1);
  vector < vector <int> > e(n + 1);
  p[0] = -1;
  for (int i = 1; i <= n; ++i) {
    cin >> p[i];
    e[p[i]].push_back(i);
  }

  function < deque < array <Modint, 3> >(int) > go = [&](int u) {
    deque < array <Modint, 3> > cur;

    int mx_2 = 0;
    for (auto v : e[u]) {
      auto t = go(v);
      if (cur.size() < t.size()) swap(t, cur);
      for (int i = 0; i < (int)t.size(); ++i) {
        cur[i] = go_dp(cur[i], t[i]);
      }
      maxi(mx_2, (int)t.size());
    }

    for (int i = 0; i < mx_2; ++i) {
      cur[i][0] += cur[i][2];
      cur[i][2] = 0;
    }

    cur.emplace_front(array <Modint, 3>{Modint(2).inv(), Modint(2).inv(), 0});

    return cur;
  };

  auto t = go(0);

  Modint ans = 0;

  for (int i = 0; i < (int)t.size(); ++i) {
    ans += t[i][1] * mypw(2, n + 1);
  }
  
  cout << ans.x << endl;

}

int main() {
  run();
  return 0;
}
