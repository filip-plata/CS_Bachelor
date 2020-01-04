#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <set>


using namespace std;

static constexpr int MAX = 500005;

typedef vector<vector<int>> Graph;

struct Node {
    int v;
    int h;
    int parent;

    Node(int val)
	: v(val), h(1), parent(val) {};
    Node() : Node(0) {};
    
};

Node sets[MAX];

int find(int x) {
    if (sets[x].parent == x) return x;
    Node &c = sets[x];

    c.parent = find(c.parent);
    
    return c.parent;
}

void union_s(int x, int y) {
    int rep_x = find(x);
    int rep_y = find(y);

    if (sets[rep_x].h > sets[rep_y].h) {
	sets[rep_y].parent = rep_x;
    } else {
	if (sets[rep_y].h > sets[rep_x].h) {
	    sets[rep_x].parent = rep_y;
	} else {
	    sets[rep_y].parent = rep_x;
	    sets[rep_x].h += 1;
	}
    }
}


int main() {
    std::ios::sync_with_stdio(false);
    
    int n, m;
    cin >> n >> m;
    vector<int> taken(n+1);
    vector<bool> visited(n+1, false);
    Graph graph(n+1);

    for (int i=1; i<=n; i++) {
	sets[i].parent = i;
	sets[i].v = i;
    }

    for (int i=1; i<=n; i++)
	cin >> taken[i];

    for (int i=0; i<m; i++) {
	int a, b;
	cin >> a >> b;
	graph[a].push_back(b);
	graph[b].push_back(a);
    }

    for (int i=1; i<=n; i++) {
	if (visited[i] || taken[i]) continue;
	queue<int> Q;
	Q.push(i);

	while (!Q.empty()) {
	    auto v = Q.front(); Q.pop();

	    for (auto ng : graph[v]) {
		if (!visited[ng]) {
		    visited[ng] = true;
		    
		    if (!taken[ng]) {
			union_s(v, ng);
			Q.push(ng);
		    }
		}
	    }
	}
    }

    int q;
    cin >> q;
    while (q--) {
	int m, w;
	cin >> m >> w;
	int p = find(w);
	bool res = false;

	if (taken[w] && w != m) {
	    cout << "NIE" << endl;
	    continue;
	}
	    

	for (auto x : graph[m]) {
	    if (find(x) == p) {
		res = true;
		break;
	    }
	}

	if (res) {

	    for (auto x : graph[m]) union_s(x, m);

	    taken[m] = 0;
	    cout << "TAK" << endl;
	} else {
	    cout << "NIE" << endl;
	}

    }
    
    return 0;
}
