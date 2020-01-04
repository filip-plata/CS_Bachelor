#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>


using namespace std;

typedef vector<vector<int>> Graph;


int main() {
    std::ios::sync_with_stdio(false);
    
    int n, m;
    cin >> n >> m;
    vector<int> taken(n+1);
    vector<bool> visited(n+1, false);
    vector<int> result;
    Graph graph(n+1);

    for (int i=1; i<=n; i++)
	cin >> taken[i];

    for (int i=0; i<m; i++) {
	int a, b;
	cin >> a >> b;
	graph[a].push_back(b);
	graph[b].push_back(a);
    }

    queue<int> Q;
    Q.push(1);

    while (!Q.empty()) {
	auto v = Q.front(); Q.pop();

	for (auto ng : graph[v]) {
	    if (!visited[ng]) {
		visited[ng] = true;
		if (taken[ng]) {
		    result.push_back(ng);
		} else {
		    Q.push(ng);
		}
	    }
	}
    }

    sort(result.begin(), result.end());

    for (auto x : result)
	cout << x << endl;
    
    return 0;
}
