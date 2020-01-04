#include <algorithm>
#include <set>
#include <vector>
#include <queue>
#include <map>
#include <iostream>


using namespace std;


static constexpr long long MAX = 500005;
typedef vector<vector<int>> Graph;
int n;
int parent[MAX];
long long s[MAX];
long long p[MAX];
long long d[MAX];
int kolor[MAX];

void clear() {
    for (int i=0; i<MAX; i++)
	kolor[i] = 0;
}

void visit(int x, Graph & g, int type) {
    kolor[x] = 1;

    if (type == 2) {
	if (x != 1) {
	    d[x] = n - 2*s[x] + d[parent[x]];
	}
    }

    for (auto ng : g[x]) {
	if (kolor[ng] == 0 && ng != parent[x]) {
	    if (type == 0)
		parent[ng] = x;
	    visit(ng, g, type);
	}
    }

    if (type == 1) {
	s[x] = 1;
	p[x] = 0;
	
	for (auto ng : g[x]) {
	    if (ng != parent[x]) {
		s[x] += s[ng];
		p[x] += (p[ng] + s[ng]);
	    }	
	}
    }
}

void DFS(Graph & g, int type) {
    clear();
    visit(1, g, type);
}


int main() {
    std::ios::sync_with_stdio(false);
    cin >> n;
    Graph g(n+1);
    parent[1] = 1;

    for (int i=0; i<n-1; i++) {
	int a, b;
	cin >> a >> b;
	g[a].push_back(b);
	g[b].push_back(a);
    }

    DFS(g, 0);
    DFS(g, 1);
    d[1] = p[1];
    DFS(g, 2);

    for (int i=1; i<=n; i++)
	cout << d[i] << endl;
    
    return 0;
}
