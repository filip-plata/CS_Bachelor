#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

typedef std::vector<std::vector<int>> Graph;
typedef std::pair<int, int> Node;


int main() {
    std::ios::sync_with_stdio(false);
    int n, m, k;
    std::cin >> n >> m >> k;

    std::vector<int> in_degree(n+1);
    std::vector<int> needed_prog(n+1);
    Graph graph(n+1);
    
    for (int i=1; i<=n; i++)
	    std::cin >> needed_prog[i];

    for (int i=0; i<m; i++) {
        int a, b;
        std::cin >> a >> b;
        in_degree[a] += 1;
        graph[b].push_back(a);
	
    }

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> projects;

    for (int i=1; i<=n; i++) {
        if (in_degree[i] == 0)
            projects.push(std::make_pair(needed_prog[i], i));
    }

    int result = 0;

    while (k--) {
	    Node node = projects.top();
        projects.pop();
	
	    for (int i : graph[node.second]) {
            in_degree[i]--;
            if (in_degree[i] == 0)
                projects.push(std::make_pair(needed_prog[i], i));
        }
        result = std::max(result, node.first);
    }

    std::cout << result << std::endl;
    
    return 0;
}
