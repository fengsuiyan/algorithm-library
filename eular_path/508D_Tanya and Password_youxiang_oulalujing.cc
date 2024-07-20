#include <bits/stdc++.h>

using namespace std;

const int N = 4e5 + 10;

int n;
vector<int> g[N];
int ind[N], outd[N];
int cur[N];
int tot;
map<string, int> mp;
vector<string> v;
char s[10];
vector<int> ans;

int hash1(char *bg, char *ed) {
  string t = "";
  for (char *p = bg; p != ed; ++p) {
    t += *p;
  }
  if (mp.count(t)) {
    return mp[t];
  }
  v.push_back(t);
  return mp[t] = ++tot;
}

void dfs(int v) {
  for (; cur[v] < g[v].size();) {
    int u = g[v][cur[v]];
    cur[v]++;
    dfs(u);
  }
  ans.push_back(v);
}

void print() {
  putchar(v[ans.back()][0]);
  for (int i = (int)ans.size() - 1; i >= 0; --i) {
    putchar(v[ans[i]][1]);
  }
  putchar('\n');
}

int main() {
  v.push_back("");
  scanf("%d", &n);
  for (int i = 0; i < n; ++i) {
    scanf("%s", s);
    int a = hash1(s, s + 2);
    int b = hash1(s + 1, s + 3);
    g[a].push_back(b);
    outd[a]++;
    ind[b]++;
  }
  int cntc = 0, cntk = 0, cnt = 0;
  int sc = 1, sk = 1;
  for (int i = 1; i <= tot; ++i) {
    if (outd[i] == ind[i] + 1) {
      sc = i;
      ++cntc;
    }
    if (outd[i] == ind[i] - 1) {
      sk = i;
      ++cntk;
    }
    if (outd[i] == ind[i]) {
      ++cnt;
    }
  }
  if (cnt == tot) {
    dfs(1);
    if (ans.size() < n + 1) {
      puts("NO");
      return 0;
    }
    puts("YES");
    print();
  }
  else if (cnt == tot - 2 && cntc == 1 && cntk == 1) {
    dfs(sc);
    if (ans.size() < n + 1) {
      puts("NO");
      return 0;
    }
    puts("YES");
    print();
  }
  else {
    puts("NO");
  }
  return 0;
}
