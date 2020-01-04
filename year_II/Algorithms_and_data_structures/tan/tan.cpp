#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

typedef long long ll;
typedef std::pair<ll, int> node_info;
static constexpr ll INF = 100000000000000ll;

typedef struct {
    ll node;
    ll cost;
    ll after_discount;
} edge;

typedef std::vector<std::vector<edge>> Graph;

int main() {
    int n, m, k;
    std::cin >> n >> m >> k;
    Graph graph(n);

    while (m--) {
	ll v, w, b, c;
	std::cin >> v >> w >> b >> c;
	graph[v].push_back({w, c, c-b});
    }

    std::vector<std::vector<ll>> distances(n, std::vector<ll>(k+1, INF));

    for (int i=0; i<=k; i++) {
	distances[0][i] = 0;
	std::priority_queue<node_info, std::vector<node_info>,
			    std::greater<node_info>> Q;

	for (int j=0; j<n; j++)
	    Q.push(std::make_pair(distances[j][i], j));

	while (!Q.empty()) {
	    if (Q.top().first != distances[Q.top().second][i]) {
		Q.pop();
		continue;
	    }
		
	    int node = Q.top().second;
	    Q.pop();
	    
	    for (auto & edge : graph[node]) {
		
		if (distances[edge.node][i] >
		    distances[node][i] + edge.cost) {
		    
		    distances[edge.node][i] =
			distances[node][i] + edge.cost;
		    Q.push(std::make_pair(
			       distances[edge.node][i], edge.node));
		}

		if (i > 0 && distances[edge.node][i] >
		    distances[node][i-1] + edge.after_discount) {
		    
		    distances[edge.node][i] =
			distances[node][i-1] + edge.after_discount;
		    Q.push(std::make_pair(distances[edge.node][i],
					  edge.node));
		}
	    }
	}
    }

    std::cout << (distances[n-1][k] != INF ? distances[n-1][k] : -1)
	      << std::endl;
    
    return 0;
}
