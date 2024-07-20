#include <bits/stdc++.h>

using namespace std;

const int K = 1, MAXN = 55;
const double EPS = 1e-12, INF = 1e9 + 10;
int tot, CX;
double X[MAXN * 2];
pair<pair<double, int>, pair<int,int> > E[MAXN * 2];

int cmp(double a, double b) {
  if (fabs(a - b) < EPS + fabs(b) * EPS) return 0;
  if (a < b) return -1;
  return 1;
}

//矩形类
struct Rect { 
  double x1, y1, x2, y2; 
} R[MAXN]; 

struct SegTree {
  short L, R, Lson, Rson, cover;
  double length[K + 1];
  void build(int, int);   
  void add(int, int, int); 
} T, A[MAXN * 4];

void SegTree::build(int l, int r) { 
  if (l > r) return;
  L = l; R = r;  cover = 0;
  memset(length, 0, sizeof(length));  
  length[0] = X[R + 1] - X[L];
  if (L == R) return ; 
  int mid = (L + R) >> 1; 
  A[Lson = tot++].build(l, mid); 
  A[Rson = tot++].build(mid + 1, r); 
}
//线段树的填删线段，只能删除之前加入过的线段
void SegTree::add(int v, int l, int r) {
  if (l > r) return;
  memset(length, 0, sizeof(length));
  if (l <= L && R <= r) { 
    cover += v;        
    if (L == R){            
      length[min<short>(cover, K)] += X[R + 1] - X[L];
    } else {
      for (int i = 0; i <= K; ++i){
        length[min(i + cover, K)] += A[Lson].length[i] + A[Rson].length[i];             
      }
    }
    return ;
  }
  int mid = (L + R) >> 1;
  if (l <= mid) A[Lson].add(v, l, r);
  if (r > mid) A[Rson].add(v, l, r);
  for (int i = 0; i <= K; ++i){
    length[min(i + cover, K)] += A[Lson].length[i] + A[Rson].length[i];
  }
}

bool doubleEqual(double x, double y) {
  return cmp(x, y) == 0;
}

bool doublelt(double x, double y) {
  return cmp(x, y) < 0;
}

//此子函数用于把坐标离散化至X数组，建立线段树，并把边的事件用y坐标排序
void discrete(Rect* R, int N) {
  int i, tx1, tx2;
  for (CX = i = 0;i < N; ++i) {
    X[CX++] = R[i].x1;
    X[CX++] = R[i].x2;
  }
  sort(X, X + CX);
  CX = unique(X, X + CX, doubleEqual) - X;
  T.build(tot = 0, CX - 2);
  for (i = 0; i < N; ++i) {
    tx1 = lower_bound(X, X + CX, R[i].x1, doublelt) - X;
    tx2 = lower_bound(X, X + CX, R[i].x2, doublelt) - X;
    E[i].second = E[i + N].second = make_pair(tx1, tx2);
    E[i].first = make_pair(R[i].y1, -1);
    E[i + N].first = make_pair(R[i].y2, 1);
  }
  sort(E, E + (N << 1));
}

//求矩形并的面积，传入储存矩形的数组R和矩形个数N
double area(Rect* R, int N) { 
  if (N == 0) return 0;
  int i, k;
  double prv; 
  double ret = 0; 
  discrete(R, N); 
  prv = E[0].first.first; 
  for(i = 0; i < (N << 1); ++i) { 
    ret += (double)T.length[K] * (E[i].first.first - prv); 
    prv = E[i].first.first; 
    T.add(-E[i].first.second, E[i].second.first, E[i].second.second - 1); 
  }
  return ret; 
}

const int N = 55;

int n;
double W, H, Ed;
double sx[N], sy[N], w[N], h[N], vx[N], vy[N];

double get(double s, double t, double vs, double vt) {
  //s + vs * x = t + vt * x;
  //s - t = (vt - vs) * x;
  //printf("get: %f %f %f %f\n", s, t, vs, vt);
  if (cmp(vs, vt) == 0) return INF;
  double x = (s - t) / (vt - vs);
  if (cmp(x, 0) < 0) return INF;
  return x;
}

double norm(double l, double r, double x) {
  if (cmp(x, l) <= 0) return l;
  if (cmp(x, r) >= 0) return r;
  return x;
}

Rect getrect(int id, double t) {
  Rect ret = {sx[id], sy[id], sx[id] + w[id], sy[id] + h[id]};
  ret.x1 += t * vx[id], ret.x2 += t * vx[id];
  ret.y1 += t * vy[id], ret.y2 += t * vy[id];
  ret.x1 = norm(0, W, ret.x1);
  ret.x2 = norm(0, W, ret.x2);
  ret.y1 = norm(0, H, ret.y1);
  ret.y2 = norm(0, H, ret.y2);
  return ret;
}

double calc(double x) {
  for (int i = 0; i < n; ++i) {
    R[i] = getrect(i, x);
  }
  return area(R, n);
}

double lagrange(double x, vector<pair<double, double> > &vp) {
  double ans = 0;
  for (int j = 0; j < vp.size(); ++j) {
    double tmp = 1;
    for (int i = 0; i < vp.size(); ++i) if (i != j) {
      tmp *= (x - vp[i].first) / (vp[j].first - vp[i].first);
    }
    ans += vp[j].second * tmp;
  }
  return ans;
}

double solve(double bg, double ed) {
  vector<pair<double, double> > vp;
  vp.push_back(make_pair(bg, calc(bg)));
  vp.push_back(make_pair((bg + ed) / 2.0, calc((bg + ed) / 2.0)));
  vp.push_back(make_pair(ed, calc(ed)));
  /*
  puts("solve");
  for (auto p : vp) printf("%f,%f ", p.first, p.second); puts("");*/
  double l = bg, r = ed;
  for (int i = 0; i < 70; ++i) {
    double md1 = (l * 2.0 + r) / 3.0, md2 = (l + r * 2.0) / 3.0;
    if (cmp(lagrange(md1, vp), lagrange(md2, vp)) > 0) {
      l = md1;
    }
    else {
      r = md2;
    }
  }
  return lagrange(l, vp);
}

int main() {
  scanf("%d", &n);
  scanf("%lf%lf%lf", &W, &H, &Ed);
  for (int i = 0; i < n; ++i) {
    scanf("%lf%lf%lf%lf%lf%lf", w + i, h + i, sx + i, sy + i, vx + i, vy + i);
  }
  vector<double> v;
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      for (int o1 = 0; o1 < 2; ++o1) {
        for (int o2 = 0; o2 < 2; ++o2) {
          double t = get(sx[i] + (o1 ? w[i] : 0), sx[j] + (o2 ? w[j] : 0), vx[i], vx[j]);
          if (cmp(t, Ed) <= 0) v.push_back(t);
          t = get(sy[i] + (o1 ? h[i] : 0), sy[j] + (o2 ? h[j] : 0), vy[i], vy[j]);
          if (cmp(t, Ed) <= 0) v.push_back(t);
        }
      }
    }
  }
  for (int i = 0; i < n; ++i) {
    for (int o1 = 0; o1 < 2; ++o1) {
      for (int o2 = 0; o2 < 2; ++o2) {
        double t = get(sx[i] + (o1 ? w[i] : 0), o2 ? W : 0, vx[i], 0);
        if (cmp(t, Ed) <= 0) v.push_back(t);
        t = get(sy[i] + (o1 ? h[i] : 0), o2 ? H : 0, vy[i], 0);
        if (cmp(t, Ed) <= 0) v.push_back(t);
      }
    }
  }
  v.push_back(0);
  v.push_back(Ed);
  sort(v.begin(), v.end());
  v.erase(unique(v.begin(), v.end(), doubleEqual), v.end());
  //for (double i : v) printf("%f ", i); puts("");
  //for (int i = 0; i < v.size(); ++i) printf("%f ", v[i]); puts("");
  double ans = W * H;
  for (int i = 0; i + 1 < v.size(); ++i) {
    ans = min(ans, solve(v[i], v[i + 1]));
  }
  printf("%.9f\n", ans);
  return 0;
}
