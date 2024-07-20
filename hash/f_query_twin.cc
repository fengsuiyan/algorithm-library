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
using ull = unsigned long long;
using ld = long double;

const int N = 2e5 + 10;
const int H = 3;

int n, m;
char s[N];

ll MUL(ll x, ll y, ll m) {
    ll c, s = 0;
    for (c = x % m; y; c= (c + c) % m, y >>= 1)
        if (y & 1) s= (s + c) % m;
    return s;
}

ll POW(ll x, ll y, ll m) {
    ll c, s = 1;
    for (c = x % m; y; c = MUL(c, c, m), y >>= 1)
        if (y & 1) s = MUL(s, c, m);
    return s;
}

bool miller_rabin(ll num) {
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (~num & 1) return false;
    const int u[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 0};
    ll e = num - 1, a, c = 0;
    while (~e & 1) e /= 2, c++;
    for (int i = 0; u[i]; i++) {
        if (num <= u[i]) return true;
        a = POW(u[i], e, num);
        if (a == 1) continue;
        for (int j = 1; a != num - 1; j++) {
            if (j == c) return false;
            a = MUL(a, a, num);
        }
    }
    return true;
}

int random_int(int l, int r) {
    static std::mt19937 gen((std::random_device())());
    std::uniform_int_distribution<int> uid{l, r};
    return uid(gen);
}

int get_random_probable_prime() {
    static int at = 1e9 - 101;
    int cnt = random_int(10, 60);
    while (cnt--) {
        at += 2;
        if (!miller_rabin(at))
            ++cnt;
    }
    return at;
}

template <int M = 2>
struct RollingHashFactory {
    int mods[M], muls[M];
    int pw[M][N];

    RollingHashFactory() {
        for (int i = 0; i < M; ++i) {
            muls[i] = get_random_probable_prime();
        }
        for (int i = 0; i < M; ++i) {
            mods[i] = get_random_probable_prime();
        }
        for (int i = 0; i < M; ++i) {
            pw[i][0] = 1;
            for (int j = 1; j < N; ++j) {
                pw[i][j] = 1ll * pw[i][j - 1] * muls[i] % mods[i];
            }
        }
    }

};

ull add(ull a, ull b, int w, RollingHashFactory <2> &rhf) {
    ull high = ((a >> 32) * rhf.pw[0][w] + (b >> 32)) % rhf.mods[0];
    ull low = (1ll * (unsigned)a * rhf.pw[1][w] + (unsigned)b) % rhf.mods[1];
    return high << 32 | low;
}

template <int M = 2>
struct RollingHash {
    RollingHashFactory <M> &rhf;
    int buf[M][N];
    int p;
    RollingHash(RollingHashFactory <M> &rhf) : rhf(rhf) {
        for (int i = 0; i < M; ++i) buf[i][0] = 0;
        p = 1;
    }

    void add(int c) {
        for (int i = 0; i < M; ++i) {
            buf[i][p] = (1ll * buf[i][p - 1] * rhf.muls[i] + c) % rhf.mods[i];
        }
        p++;
    }

    int query_one(int l, int r) {
        l -= 1;
        const int i = 0;
        int v = (buf[i][r] - 1ll * buf[i][l] * rhf.pw[i][r - l]) % rhf.mods[i];
        if (v < 0) v += rhf.mods[i];
        return v;
    }

    ull query_twin(int l, int r) {
        l -= 1;
        assert(l <= r);
        assert(M == 2);
        ull h = 0;
        for (int i = 0; i < M; ++i) {
            int v = (buf[i][r] - 1ll * buf[i][l] * rhf.pw[i][r - l]) % rhf.mods[i];
            if (v < 0) v += rhf.mods[i];
            h = h << 32 | v;
        }
        return h;
    }

    array <int, M> query(int l, int r) {
        l -= 1;
        assert(l <= r);
        array <int, M> h;
        for (int i = 0; i < M; ++i) {
            h[i] = (buf[i][r] - buf[i][l] * rhf.pw[i][r - l]) % rhf.mods[i];
            if (h[i] < 0) h[i] += rhf.mods[i];
        }
        return h;
    }

};

RollingHashFactory <> rhf;
vector <RollingHash <> > rhs(26, RollingHash <>(rhf));

vector <ll> get(int x, int len) {
    vector <ll> ret;
    for (int i = 0; i < 26; ++i) {
        ret.push_back(rhs[i].query_twin(x, x + len - 1));
    }
    return ret;
}

bool solve(int x, int y, int len) {
    vector <ll> a = get(x, len);
    vector <ll> b = get(y, len);
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    return a == b;
}

void run() {
    scanf("%d%d", &n, &m);
    scanf("%s", s + 1);
    debug(rhf.mods[0], rhf.mods[1]);

    for (int i = 0; i < 26; ++i) {
        for (int j = 1; j <= n; ++j) {
            rhs[i].add(s[j] == 'a' + i ? 1 : 0);
        }
    }

    while (m--) {
        int x, y, len;
        scanf("%d%d%d", &x, &y, &len);
        puts(solve(x, y, len) ? "YES" : "NO");
    }
}

int main() {
    run();
    return 0;
}
