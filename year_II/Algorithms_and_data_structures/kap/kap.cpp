#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>
#include <map>

typedef unsigned long long ull;
typedef long long ll;
typedef std::size_t node;
typedef std::pair<ull, node> node_info;
typedef std::pair<node, ull> edge;
typedef std::vector<std::set<edge>> Graph;
typedef std::pair<ull, ull> point;

static constexpr ull INF = 4000000005ll;


edge make_edge(node a, node b, std::vector<point> &data) {
    point u = data[a], v = data[b];
    ll dist_x = std::abs((ll)u.first - (ll)v.first);
    ll dist_y = std::abs((ll)u.second - (ll)v.second);
    ll dist = std::min(dist_x, dist_y);
    return std::make_pair(b, (ull)dist);
}


int main() {
    std::ios::sync_with_stdio(false);
    std::size_t n, x, y;
    std::cin >> n;
    Graph graph(n+1);

    std::vector<point> data(n+1);
    std::vector<std::pair<point, node>> x_cord(n+1), y_cord(n+1);

    for (node i=1; i<=n; i++) {
        std::cin >> x >> y;
        point p_x = std::make_pair(x, y);
        point p_y = std::make_pair(y, x);

        x_cord[i] = std::make_pair(p_x, i);
        y_cord[i] = std::make_pair(p_y, i);
        data[i] = p_x;
    }

    std::sort(x_cord.begin(), x_cord.end());
    std::sort(y_cord.begin(), y_cord.end());

    for (node v = 1; v <= n; v++) {
        node c_x = x_cord[v].second;
        node c_y = y_cord[v].second;
        if (v < n) {
            node r = x_cord[v+1].second;
            graph[c_x].insert(make_edge(c_x, r, data));
            r = y_cord[v+1].second;
            graph[c_y].insert(make_edge(c_y, r, data));
        }
        if (v > 1) {
            node l = x_cord[v-1].second;
            graph[c_x].insert(make_edge(c_x, l, data));
            l = y_cord[v-1].second;
            graph[c_y].insert(make_edge(c_y, l, data));
        }
    }


    std::vector<ull> distances(n+1, INF);

    distances[1] = 0;
    std::priority_queue<node_info, std::vector<node_info>,
            std::greater<node_info>> Q;

    for (node j=1; j<=n; j++)
        Q.push(std::make_pair(distances[j], j));

    while (!Q.empty()) {
        if (Q.top().first != distances[Q.top().second]) {
            Q.pop();
            continue;
        }

        node v = Q.top().second;
        Q.pop();

        for (auto & edge : graph[v]) {

            if (distances[edge.first] >
                distances[v] + edge.second) {

                distances[edge.first] =
                        distances[v] + edge.second;
                Q.push(std::make_pair(
                        distances[edge.first], edge.first));
            }
        }
    }

    std::cout << distances[n] << std::endl;
    
    return 0;
}
