#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>

using namespace std;

typedef vector<vector<pair<int, int>>> Graph;

static const int INF = 100000000;
static const int MAX = 500005;

int dist[MAX];
int n, m;

void do_bfs(Graph &graph, int col) {
    vector<int> distances(2*n + 1, INF);
    distances[1] = 0;
    distances[n+1] = 0;
   vector<int> visited(n*2+1, 0); // first for color 0 edge
   queue<pair<int, int>> Q;
   Q.push(make_pair(1, col)); // starts color 'col'
   
   while (!Q.empty()) {
	auto p = Q.front();
	Q.pop();

	for (auto ng : graph[p.first]) {
	    if (visited[ng.first] == 0 && ng.second == 0 && p.second == 1) {
		visited[ng.first] = 1;
		distances[ng.first] = min(distances[ng.first], 1 + distances[p.first+n]);
		Q.push(ng);
	    }
	    if (visited[ng.first + n] == 0 && ng.second == 1 && p.second == 0) {
		visited[ng.first + n] = 1;
		distances[ng.first + n] = min(distances[ng.first + n], 1 + distances[p.first]);
		Q.push(ng);
	    }
	}
    }

   for (int i=2; i<=n; i++) {
       dist[i] = min(dist[i], distances[i]);
       dist[i] = min(dist[i], distances[i+n]);
   }
}


int main() {
    cin >> n >> m;
    Graph graph(n+1);
    for (int i=2; i<=n; i++)
	dist[i] = INF;
    dist[1] = 0;

    for (int i=0; i<m; i++) {
	int a, b, c;
	cin >> a >> b >> c;
	graph[a].push_back(make_pair(b, c));
	graph[b].push_back(make_pair(a, c));
    }

    do_bfs(graph, 0);
    do_bfs(graph, 1);

    for (int i=2; i<=n; i++) {
	if (dist[i] == INF)
	    cout << -1 << endl;
	else
	    cout << dist[i] << endl;
    }
    
    return 0;
}
