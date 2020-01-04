#include <algorithm>
#include <set>
#include <vector>
#include <queue>
#include <map>
#include <iostream>


typedef long long ll;

static constexpr ll INF = 1000000000000000000LL;

using namespace std;

typedef vector<vector<pair<ll, ll>>> Graph;

int main() {
    std::ios::sync_with_stdio(false);
    
    int n, m;
    cin >> n;
    vector<ll> prices(n+1);
    Graph g(n+1);
    multiset<ll> p;
    ll result = INF;

    for (int i=1; i<=n; i++) {
	cin >> prices[i];
	p.insert(prices[i]);
	g[i].push_back(make_pair(i, 4*INF));
    }

    cin >> m;
    for (int i=0; i<m; i++) {
	ll u, v, b;
	cin >> u >> v >> b;
	g[v].push_back(make_pair(u, b));
	g[u].push_back(make_pair(v, b));
    }

    for (int i=1; i<=n; i++) {
	for (auto x : g[i]) {
	    p.erase(prices[x.first]);
	    result = min(result, prices[i] + prices[x.first] + x.second*2);
	}

	if (!p.empty())
	    result = min(result, prices[i] + *(p.begin()));

	for (auto x : g[i]) {
	    p.insert(prices[x.first]);
	}
    }

    cout << result << endl;
    
    return 0;
}
