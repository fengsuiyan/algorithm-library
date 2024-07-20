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

#define X first
#define Y second

using ll = long long;
using ld = long double;

const int N = 1e5 + 10;
const int L = 2, MAG = 310;
const int LOGN = 20;
const int inf = 1e9 + 10;

int q;

struct Trie {
    struct Node {
        int ch[L];
        int mat;
        int mn;
        Node() {
            memset(ch, 0, sizeof(ch));
            mat = 0;
            mn = inf;
        }
    } pool[N * L * LOGN];

    int root[MAG];
    int node_cnt;

    void init() {
        pool[0] = Node();
        for (int i = 1; i < MAG; ++i) root[i] = i, pool[root[i]] = Node();
        node_cnt = MAG - 1;
        //pool[node_cnt] = Node();
        //root = node_cnt = 1;
        //pool[node_cnt] = Node();
    }

    int get_node() {
        pool[++node_cnt] = Node();
        return node_cnt;
    }
    
    vector <int> get(int x) {
        vector <int> ret;
        for (int i = 0; i < 18; ++i) {
            ret.push_back(x >> i & 1);
        }
        reverse(ret.begin(), ret.end());
        return ret;
    }

    void insert(int who, int x) {
        vector <int> s = get(x);
        int p = root[who];
        for (int i = 0; i < s.size(); ++i) {
            mini(pool[p].mn, x);
            int c = s[i];
            int &np = pool[p].ch[c];
            if (np == 0) np = get_node();
            p = np;
        }
        mini(pool[p].mn, x);
        pool[p].mat = 1;
    }

    int query(int who, int x, int tot) {
        debug(who, x, tot);
        vector <int> s = get(x);
        int mx = (tot - x);
        int p = root[who];
        int nwx = 0;
        int h = 1 << 17;
        for (int i = 0; i < s.size(); ++i, h >>= 1) {
            int np1 = pool[p].ch[1], np0 = pool[p].ch[0];
            debug(np1, np0);
            if (s[i] == 0) {
                if (np1 != 0 && pool[np1].mn <= mx) {
                    p = np1;
                    nwx += h;
                } else if (np0 != 0 && pool[np0].mn <= mx) {
                    p = np0;
                } else {
                    p = 0;
                }
            } else {
                if (np0 != 0 && pool[np0].mn <= mx) {
                    p = np0;
                } else if (np1 != 0 && pool[np1].mn <= mx) {
                    p = np1;
                    nwx += h;
                } else {
                    p = 0;
                }
            }
            debug(pool[p].mn);
            if (p == 0) return -1;
        }
        return nwx;
    }

} tr;

vector <int> p[N];
set <int> st;

void init() {
    for (int i = 1; i < N; ++i) {
        for (int j = i; j < N; j += i) {
            p[j].push_back(i);
        }
    }
}

void add(int x) {
    for (auto y : p[x]) {
        if (y < MAG) {
            debug(x, y);
            tr.insert(y, x);
        }
    }

    st.insert(x);
}

int query(int x, int k, int s) {
    if (x % k == 0) {
        if (k < MAG) {
            return tr.query(k, x, s);
        } else {
            int mx = -1;
            int who = -1;
            for (auto v = k; v < N; v += k) {
                if (st.count(v)) {
                    if (x + v <= s) {
                        if (mx < (x ^ v)) {
                            mx = (x ^ v);
                            who = v;
                        }
                    }
                }
            }
            return who;
        }
    } else {
        return -1;
    }
}

void run() {
    cin >> q;

    init();
    debug(p[5]);
    tr.init();
    while (q--) {
        int t, u, x, k, s;
        cin >> t;
        if (t == 1) {
            cin >> u;
            add(u);
        } else {
            cin >> x >> k >> s;
            cout << query(x, k, s) << endl;
        }
    }
}

int main() {
    run();
    return 0;
}
