#include <bits/stdc++.h>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/assoc_container.hpp>

using namespace std;
using namespace __gnu_pbds;

template <class T> void mini(T &l, T r) {l = min(l, r);}
template <class T> void maxi(T &l, T r) {l = max(l, r);}

template <class T> using Tree = tree <T, null_type, less<T>, rb_tree_tag,tree_order_statistics_node_update>;

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

void run() {
  Tree <int> s;
  s.insert(12);
  s.insert(505);
  s.insert(30);
  s.insert(1000);
  s.insert(10000);
  s.insert(100);
  
  assert(*s.find_by_order(0) == 12);
  assert(*s.find_by_order(1) == 30);
  assert(*s.find_by_order(2) == 100);
  assert(*s.find_by_order(3) == 505);
  assert(*s.find_by_order(4) == 1000);
  assert(*s.find_by_order(5) == 10000);
  assert(s.find_by_order(6) == s.end());

  debug(s);

  assert(s.order_of_key(10) == 0);
  assert(s.order_of_key(12) == 0);
  assert(s.order_of_key(15) == 1);
  assert(s.order_of_key(30) == 1);
  assert(s.order_of_key(99) == 2);
  assert(s.order_of_key(100) == 2);
  assert(s.order_of_key(505) == 3);
  assert(s.order_of_key(1000) == 4);
  assert(s.order_of_key(10000) == 5);
  assert(s.order_of_key(9999999) == 6);

  // Erase an entry.
  s.erase(30);

  // The order of the keys should be: 12, 100, 505, 1000, 10000.
  assert(*s.find_by_order(0) == 12);
  assert(*s.find_by_order(1) == 100);
  assert(*s.find_by_order(2) == 505);
  assert(*s.find_by_order(3) == 1000);
  assert(*s.find_by_order(4) == 10000);
  assert(s.find_by_order(5) == s.end());

  // The order of the keys should be: 12, 100, 505, 1000, 10000.
  assert(s.order_of_key(10) == 0);
  assert(s.order_of_key(12) == 0);
  assert(s.order_of_key(100) == 1);
  assert(s.order_of_key(505) == 2);
  assert(s.order_of_key(707) == 3);
  assert(s.order_of_key(1000) == 3);
  assert(s.order_of_key(1001) == 4);
  assert(s.order_of_key(10000) == 4);
  assert(s.order_of_key(100000) == 5);
  assert(s.order_of_key(9999999) == 5);
}

int main() {
  run();
  return 0;
}
