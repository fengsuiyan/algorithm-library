#include <bits/stdc++.h>

using namespace std;
using llint = long long;

const int N = 1e5 + 10;

int n;
char s[N];
vector <int> e[N];
int a[N], b[N];

struct Node {
  int a, b, mna, mnb;
};
llint ans;

namespace centroid {
  vector <int> sz, mark;
  int tot, mins, rt;
  vector <int> vtx;
  vector <int> vt[N];
  vector <Node> info;

  void init(int n) {
    sz.resize(n);
    info.resize(n);
    mark.assign(n, 0);
    ans = 0;
  }

  void get_center(int u, int f, vector <int> G[]) {
    int mx = 0;
    sz[u] = 1;
    for (auto &&v : G[u]) if (v != f && !mark[v]) {
      get_center(v, u, G);
      sz[u] += sz[v];
      mx = max(mx, sz[v]);
    }
    mx = max(mx, tot - sz[u]);
    if (mx < mins) mins = mx, rt = u;
  }

  void dfs(int u, int f, int a, int b, int mna, int mnb, vector <int> G[], int tp) {
    //cout << "dfs " << u << " " <<  f << endl;
    int na = a + (s[u] == '(' ? -1 : 1), nb = b + (s[u] == ')' ? -1 : 1);
    mna = min(na, mna);
    mnb = min(nb, mnb);
    vtx.push_back(u);
    vt[tp].push_back(u);
    info[u] = Node{na, nb, mna, mnb};
    for (auto &&v : G[u]) if (v != f && !mark[v]) {
      dfs(v, u, na, nb, mna, mnb, G, tp);
    }
  }

  void run(int u, int _tot, vector <int> G[]) {
    //cout << "run " <<  u << " " << _tot << endl;
    tot = _tot;
    mins = tot * 2, rt = u;
    get_center(u, -1, G);
    mark[u = rt] = true;
    get_center(u, -1, G);
    vtx.clear();
    int b = s[u] == ')' ? -1 : 1;
    for (auto v : G[u]) if (!mark[v]) {
      vt[v].clear();
      dfs(v, u, 0, b, 0, b, G, v);
    }
    //cout << "run  rt " << rt << endl;
    map <int, int> mpr;
    for (auto v : vtx) {
      /*
      if (info[v].a <= info[v].mna) {
        mpl[info[v].a]++;
      }*/
      if (info[v].b <= info[v].mnb) {
        mpr[info[v].b]++;
      }
    }
    if (b < 0) {
      mpr[b]++;
    }
    //cout << "bf " <<ans << endl;
    //rt
    ans += mpr[0];
    //cout << "ver : " << rt << endl;
    //for (auto v : vtx) cout << v << " "; cout << endl;
    //cout << "mpr " << endl;
    //for (auto p : mpr) cout << p.first <<  "," << p.second << " "; cout << endl;
      for (auto v : G[u]) if (v != u && !mark[v]) {
      //cout << v << " : " << endl;
      //for (auto p : mpr) cout << p.first <<  "," << p.second << " "; cout << endl;
      for (auto j : vt[v]) {
        if (info[j].b <= info[j].mnb) mpr[info[j].b]--;
      }
      //for (auto p : mpr) cout << p.first <<  "," << p.second << " "; cout << endl;
      for (auto j : vt[v]) {
        if (info[j].a <= info[j].mna)
          ans += mpr[info[j].a];
      }
      for (auto j : vt[v]) {
        if (info[j].b <= info[j].mnb) mpr[info[j].b]++;
      }
      //for (auto p : mpr) cout << p.first <<  "," << p.second << " "; cout << endl;
    }
    //cout << "af " << ans << endl;
    for (auto v : G[u]) if (!mark[v]) {
      run(v, sz[v], G);
    }
  }

}

void run() {
  scanf("%d", &n);
  scanf("%s", s);
  for (int i = 1; i < n; ++i) {
    scanf("%d%d", a + i, b + i);
    --a[i], --b[i];
    e[a[i]].push_back(b[i]);
    e[b[i]].push_back(a[i]);
  }
  centroid::init(n);
  centroid::run(0, n, e);
  printf("%lld\n", ans);
}

int main() {
  run();
  return 0;
}
