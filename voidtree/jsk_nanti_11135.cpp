#include<bits/stdc++.h>
using namespace std;
const int MAXN = 100005;
vector<int> Map[MAXN];
typedef long long LL;
int aa[MAXN];
class voidTree{
public:
	#define mk make_pair
	static const int MAXN = 100005,LOG = 17;
	int dfn[MAXN],out[MAXN],dep[MAXN],f[MAXN][LOG + 1];
	LL g[MAXN],ct[MAXN];
	bool flag[MAXN];
	vector<pair<int,LL> > tree[MAXN];
	vector<pair<int,int> > a;
	stack<int> s;
	int tot,root,n;
	void set(int _n)
	{
		n = _n;
		root = 1; tot = 0; dfs(1,0);
		for (int i=1;i<=LOG;i++)
		{
			for (int j=1;j<=n;j++) f[j][i] = f[f[j][i-1]][i-1];
		}
	}
	void dfs(int u,int fa)
	{
		f[u][0] = fa;
		dfn[u] = ++tot;
		dep[u] = dep[fa] + 1;
		for (auto v:Map[u]) 
			if (v!=fa) dfs(v,u);
		out[u] = tot;
	}
	int lca(int u,int v)
	{
		if (dep[u] < dep[v]) swap(u,v);
		for (int i=LOG;i>=0;i--)
			if (dep[f[u][i]] >= dep[v]) u = f[u][i];
		if (u == v) return u;
		for (int i=LOG;i>=0;i--)
			if (f[u][i] != f[v][i]){
				u = f[u][i];
				v = f[v][i];
			}
		return f[u][0];
	}
	inline bool check(int u,int v)
	{
		return (dfn[v]>=dfn[u]+1 && dfn[v]<=out[u]);
	}
	LL dp(int u,int fa = 0)
	{
		LL ret = 0;
		ct[u] = 0;
		g[u] = 0;
		for (auto x:tree[u]){
			int v = x.first;
			if (v==fa) continue;
			ret += dp(v,u);
			ret += ct[v] * g[u] + ct[u] * (g[v] + ct[v] * x.second);
			if (flag[u]){
				ret += g[v] + ct[v] * x.second;					
			}
			g[u] += g[v] + ct[v] * x.second;
			ct[u] += ct[v];			
		}
		if (flag[u]) ct[u]++;
		return ret;
	}
	void create(vector<int> &nodes)
	{
		a.clear();
		root = nodes[0];
		//for (int i=1;i<=n;i++) flag[i]=false;//memset(flag,0,sizeof flag);
		//for (int i=1;i<=n;i++) tree[i].clear();
		for (auto x:nodes) {
			a.push_back(make_pair(dfn[x],x));
		}
		sort(a.begin(),a.end());
		for (int i=1,m=a.size();i<m;i++) {
			int x = lca(a[i].second,a[i-1].second);
			a.push_back(make_pair(dfn[x],x));
		}
		sort(a.begin(),a.end());
		for (auto x:a) {
			flag[x.second] = false;
			tree[x.second].clear();
		}
		for (auto x:nodes) flag[x] = true;
		auto new_end = unique(a.begin(),a.end()); a.erase(new_end,a.end());
		while (!s.empty()) s.pop();
		for (auto x:a)
		{
			while (!s.empty() && !check(s.top(),x.second)) s.pop();
			if (s.empty()) s.push(x.second);
			else {
				tree[s.top()].push_back(mk(x.second,dep[x.second] - dep[s.top()]));//s.top()是x.second的父亲 
				tree[x.second].push_back(mk(s.top(),dep[x.second] - dep[s.top()]));//非双向则注释掉 
				s.push(x.second);
			}
		}
		
	//	for (int i=1;i<=n;i++)
		//	for (auto v:tree[i])
	//			if (dfn[v.first] > dfn[i]) cout<<aa[i]<<" -> "<<aa[v.first]<<" cost = "<<v.second<<endl; 
	}
} VT;

LL f[MAXN];
vector<int> lst[MAXN];
vector<int> tmp;
int main()
{
	freopen("1.in","r",stdin);
//	freopen("1.out","w",stdout);
	int n,mx = -1;
	scanf("%d",&n);
	for (int i=1;i<=n;i++){
		scanf("%d",aa+i);
		mx = max(aa[i],mx);
		lst[aa[i]].push_back(i);
	}
	for (int i=1;i<n;i++){
		int x,y;
		scanf("%d%d",&x,&y);
		Map[x].push_back(y);
		Map[y].push_back(x);
	}
	VT.set(n);
	LL ans = 0;
	for (int i=mx;i>=1;i--) {
		tmp.clear();
		for (int j=1;i*j<=mx;j++)
			for (auto x:lst[i*j]) tmp.push_back(x);
		//cout<<i<<endl;
		if (tmp.size() < 2) continue;
		VT.create(tmp);
		f[i] = VT.dp(VT.root);
		//cout<<f[i]<<"->";
		if (i!=1){
			for (int j=i+i;j<=mx;j+=i) f[i]-=f[j];
			ans += f[i];
		}
		else ans = f[i] - ans;
		//cout<<f[i]<<endl<<"========"<<endl;
	}
	printf("%lld\n",ans);
}
