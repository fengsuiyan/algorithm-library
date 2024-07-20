#include <bits/stdc++.h>

using namespace std;

const int N = 1e5 + 10;
const int INF = 1e7 + 10;

char s[N];
int n;
vector<pair<int, int> > ans;

namespace SA {
int sa[N], rk[N], ht[N], s[N << 1], t[N << 1], p[N], cnt[N], cur[N];
#define pushS(x) sa[cur[s[x]]--] = x
#define pushL(x) sa[cur[s[x]]++] = x
#define inducedSort(v) fill_n(sa, n, -1); fill_n(cnt, m, 0); \
  for (int i = 0; i < n; ++i) cnt[s[i]]++; \
  for (int i = 1; i < m; ++i) cnt[i] += cnt[i - 1]; \
  for (int i = 0; i < m; ++i) cur[i] = cnt[i] - 1; \
  for (int i = n1 - 1; ~i; --i) pushS(v[i]); \
  for (int i = 1; i < m; ++i) cur[i] = cnt[i - 1]; \
  for (int i = 0; i < n; ++i) if (sa[i] > 0 && t[sa[i] - 1]) pushL(sa[i] - 1); \
  for (int i = 0; i < m; ++i) cur[i] = cnt[i] - 1; \
  for (int i = n - 1; ~i; --i) if (sa[i] > 0 && !t[sa[i] - 1]) pushS(sa[i] - 1)

void sais(int n, int m, int *s, int *t, int *p) {
  int n1 = t[n - 1] = 0, ch = rk[0] = -1, *s1 = s + n;
  for (int i = n - 1; ~i; --i) t[i] = s[i] == s[i + 1] ? t[i + 1] : s[i] > s[i + 1];
  for (int i = 1; i < n; ++i) rk[i] = t[i - 1] && !t[i] ? (p[n1] = i, n1++) : -1;
  inducedSort(p);
  for (int i = 0, x, y; i < n; ++i) if (~(x = rk[sa[i]])) {
    if (ch < 1 || p[x + 1] - p[x] != p[y + 1] - p[y]) ch++;
    else for (int j = p[x], k = p[y]; j <= p[x + 1]; ++j, ++k)
      if ((s[j] << 1 | t[j]) != (s[k] << 1 | t[k])) {ch++; break;}
    s1[y = x] = ch;
  }
  if (ch + 1 < n1) sais(n1, ch + 1, s1, t + n, p + n1);
  else for (int i = 0; i < n1; ++i) sa[s1[i]] = i;
  for (int i = 0; i < n1; ++i) s1[i] = p[sa[i]];
  inducedSort(s1);
}
template<typename T>
int mapCharToInt(int n, const T *str) {
  int m = *max_element(str, str + n);
  fill_n(rk, m + 1, 0);
  for (int i = 0; i < n; ++i) rk[str[i]] = 1;
  for (int i = 0; i < m; ++i) rk[i + 1] += rk[i];
  for (int i = 0; i < n; ++i) s[i] = rk[str[i]] - 1;
  return rk[m];
}
template<typename T>
void suffixArray(int n, const T *str) {
  int m = mapCharToInt(++n, str);
  sais(n, m, s, t, p);
  for (int i = 0; i < n; ++i) rk[sa[i]] = i;
  for (int i = 0, h = ht[0] = 0; i < n - 1; ++i) {
    int j = sa[rk[i] - 1];
    while (i + h < n && j + h < n && s[i + h] == s[j + h]) ++h;
    if (ht[rk[i]] = h) h--;
  }
}
};

int main() {
  scanf("%s", s);
  n = strlen(s);
  SA::suffixArray(n, s);
  // rk[i] : starts from 1
  for (int i = 0; i < n; ++i) {
    printf("%d ", SA::rk[i]);
  }
  puts("");
  // ht[i] : sa[i] & sa[i - 1]
  for (int i = 1; i <= n; ++i) {
    printf("%d ", SA::ht[i]);
  }
  puts("");
  for (int i = 1; i <= n; ++i) {
    printf("%d ", SA::sa[i]);
  }
  puts("");
  int p = SA::rk[0];
  int mn = INF;
  ans.push_back(make_pair(n - SA::sa[p], 1));
  int k = p + 1;
  int mn2 = INF;
  for (int i = p - 1; i >= 1; --i) {
    mn = min(mn, SA::ht[i + 1]);
    if (mn == (n - SA::sa[i])) {
      for (; k <= n; ++k) {
        mn2 = min(mn2, SA::ht[k]);
        if (mn2 < mn) {
          break;
        }
      }
      ans.push_back(make_pair(n - SA::sa[i], k - i));
    }
  }
  printf("%d\n", ans.size());
  sort(ans.begin(), ans.end());
  for (int i = 0; i < ans.size(); ++i) {
    printf("%d %d\n", ans[i].first, ans[i].second);
  }
  return 0;
}
