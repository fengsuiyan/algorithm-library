#include <bits/stdc++.h>

using namespace std;

template <class T> void mini(T &l, T r) {l = min(l, r);}
template <class T> void maxi(T &l, T r) {l = max(l, r);}

string to_string(string s) {
  return '"' + s + '"';
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

const int N = 1e5 + 10, L = 2;

int n;
ll l;
string s[N];

ll get(ll l) {
  return 1 << __builtin_ctz(l);
}

struct Trie {
  struct Node {
    int level;
    int ch[L];
    int mat;
    Node(int lv = 0) {
      memset(ch, 0, sizeof(ch));
      mat = 0;
      level = lv;
    }
  } pool[N * L];

  int root;
  int node_cnt;
  
  void init() {
    root = node_cnt = 1;
    pool[node_cnt] = Node();
  }

  int get_node(int level = 0) {
    pool[++node_cnt] = Node(level);
    return node_cnt;
  }

  void insert(const char *s) {
    int p = root;
    for (int i = 0; s[i]; ++i) {
      int c = s[i] - '0';
      int &np = pool[p].ch[c];
      if (np == 0) np = get_node(pool[p].level + 1);
      p = np;
    }
    pool[p].mat = 1;
    
  }

  ll dfs(int p) {
    debug(p, pool[p].ch[0], pool[p].ch[1], root, pool[p].mat);
    ll ret = 0;
    if (pool[p].mat == 1) {debug(p, 0); return 0;}
    for (int i = 0; i < L; ++i) {
      if (pool[p].ch[i]) ret ^= dfs(pool[p].ch[i]);
      else ret ^= get(::l - pool[p].level);
    }
    debug(p, ret);
    return ret;
  }

} trie;

struct Brute {
  static const int N = 256;
  int sg[N];
  void init() {
    sg[0] = 0;
    sg[1] = 1;
    for (int i = 2; i < N; ++i) {
      set <int> st;
      for (int j = 0; j < i; ++j) {
        int ret = 0;
        for (int t = i - 1; t >= i - j; --t) {
          ret ^= sg[t];
        }
        st.insert(ret);
      }
      for (int x = 0; ; ++x) if (!st.count(x)) {sg[i] = x; break;}
    }
    for (int i = 0; i < N; ++i) cout << sg[i] << " "; cout << endl;
  }
} bt;


void run() {
  //bt.init();

  cin >> n >> l;
  for (int i = 0; i < n; ++i) cin >> s[i];

  trie.init();

  for (int i = 0; i < n; ++i) trie.insert(s[i].c_str());
  
  debug(trie.pool[5].ch[0], trie.pool[5].ch[1]);

  puts(trie.dfs(trie.root) ? "Alice" : "Bob");

}

int main() {
  run();
  return 0;
}
