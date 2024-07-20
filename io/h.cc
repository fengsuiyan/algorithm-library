#include <bits/stdc++.h>

using namespace std;
using namespace placeholders;

template <class T> void mini(T &l, T r) {l = min(l, r);}
template <class T> void maxi(T &l, T r) {l = max(l, r);}

template <class TH> void _dbg(const char *sdbg, TH h) {
  cerr << sdbg << "=" << h << "\n";
}
template <class TH, class ...TA> void _dbg(const char *sdbg, TH h, TA ...a) {
  while (*sdbg != ',') cerr << *sdbg++; cerr << "=" << h << ","; _dbg(sdbg + 1, a...);
}
template <class T> ostream &operator<<(ostream &os, vector <T> v) {
  os << "[";
  for (auto vv : v) os << vv << ",";
  return os << "]";
}
template <class L, class R> ostream &operator<<(ostream &os, pair <L, R> p) {
  return os << "(" << p.first << "," << p.second << ")";
}

#define eput(...) _dbg(#__VA_ARGS__, __VA_ARGS__)

const int N = 1e5 + 10;

int n, k;
int a[N];
vector <int> e[N];
int dp[N][2];

void dfs(int u) {
  int sum = 0, mx = -1;
  for (auto v : e[u]) {
    dfs(v);
    mx = max(dp[v][0] - dp[v][1], mx);
    sum += dp[v][1];
  }
  dp[u][0] = sum;
  dp[u][1] = sum + mx + 1;
  dp[u][1] = max(dp[u][0], dp[u][1]);
}
namespace IO  
{  
const int U=40*1024*1024;  
char buf[U];  
int ptr,sz;  
void begin()  
{  
    ptr=0;  
    sz=fread(buf,1,U,stdin);  
}  
template<typename T>  
inline bool scan_d (T&t)  
{  
    while (ptr<sz&&buf[ptr]!='-'&&(buf[ptr]<'0'||buf[ptr]>'9'))ptr++;  
    if (ptr>=sz)return false;  
    bool sgn = false;  
    if (buf[ptr]=='-')sgn = true,ptr++;  
    for (t = 0; ptr<sz&&'0'<= buf[ptr] &&buf[ptr] <= '9'; ptr++)  
        t=t*10+buf[ptr] - '0';  
    if (sgn)t=-t;  
    return true;  
}  
}  
using namespace IO;  

void run() {
  int t;
  IO::begin();
  scan_d(t);
  while (t--) {
    scan_d(n), scan_d(k);
    for (int i = 1; i <= n; ++i) e[i].clear();
    for (int i = 2; i <= n; ++i) {
      scan_d(a[i]);
      //assert(a[i] < i);
      e[a[i]].push_back(i);
    }

    dfs(1);

    int a = dp[1][1];
    if (k <= 2 * a) {
      printf("%d\n", (k + 1) / 2);
    } else {
      printf("%d\n", a + (k - 2 * a));
    }
  }
}

int main() {
  run();
  return 0;
}
