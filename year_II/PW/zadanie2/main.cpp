#include <iostream>
#include <map>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <thread>
#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <mutex>

template<class Iterator, class Function>
void pfor_each(Iterator first, Iterator last,
               Function && f, const int nthreads = 1) {
    const int group = std::max(1, (const int)(last - first)/nthreads);
    std::vector<std::thread> threads;

    for (Iterator it = first; it < last; it += group) {
        threads.push_back(std::thread([=, &f](){std::for_each(it, std::min(it+group, last), f);}));
    }

    std::for_each(threads.begin(), threads.end(), [](std::thread& x){x.join();});
}

typedef int V;
typedef std::unordered_map<V, double> Connectness;
typedef std::unordered_map<V, std::vector<V>> Graph;


void do_map(V a,
            std::unordered_map<V,V> &mapping,
            std::unordered_map<V,V> &unmapping) {
    static int next = 0;
    
    if (mapping.count(a) == 0) {
        mapping[a] = next;
        unmapping[next] = a;
        next++;
    }
}

void add_if_not_encountered(V a, std::unordered_set<V> &previous_vertices,
                            std::vector<V> &vertices) {
    if (previous_vertices.find(a) == previous_vertices.end()) {
        vertices.push_back(a);
    }

    previous_vertices.insert(a);
}

void read_input(const std::string & inFile, Connectness & BC,
                Graph & graph, std::vector<V> & vertices,
                std::unordered_map<V, std::unique_ptr<std::mutex>> & mutexes,
                std::unordered_map<V,V> &mapping,
                std::unordered_map<V,V> &unmapping) {
    std::unordered_set<V> previous_vertices;
    std::fstream input(inFile, std::fstream::in);
    V a, b;

    while (input >> a >> b) {
        do_map(a, mapping, unmapping);
        do_map(b, mapping, unmapping);
        BC[mapping[a]] = 0; BC[mapping[b]] = 0;
        mutexes.insert(std::make_pair(mapping[a], std::make_unique<std::mutex>()));
        mutexes.insert(std::make_pair(mapping[b], std::make_unique<std::mutex>()));

        add_if_not_encountered(a, previous_vertices, vertices);
        add_if_not_encountered(b, previous_vertices, vertices);

        if (graph.find(mapping[a]) != graph.end()) {
            graph[mapping[a]].push_back(mapping[b]);
        } else {
            graph[mapping[a]] = std::vector<V>({mapping[b]});
        }

        if (graph.find(mapping[b]) == graph.end()) {
            graph[mapping[b]] = std::vector<V>();
        }
    }

}

void print_output(const std::string & outFile,
                  Connectness & BC, Graph &graph,
                  std::unordered_map<V, V> &mapping,
                  std::vector<V> &vertices) {
    std::fstream output(outFile, std::fstream::out);
    std::sort(vertices.begin(), vertices.end());

    for (auto node : vertices) {
        if (graph[mapping[node]].size() > 0) {
            output << node << " " << BC[mapping[node]] << '\n';
        }
    }
}

void Brandes(Graph & graph, Connectness & BC,
             const std::vector<V> & vertices, V s,
             std::unordered_map<V, std::unique_ptr<std::mutex>> & mutexes) {

    std::stack<V> S;
    double sigma[vertices.size()];
    double d[vertices.size()];
    double delta[vertices.size()];
    std::vector<V> P[vertices.size()];

    for (size_t w = 0; w< vertices.size(); w++) {
        P[w] = std::vector<V>();
        sigma[w] = 0;
        d[w] = -1;
        delta[w] = 0;
    }

    sigma[s] = 1;
    d[s] = 0;
    std::queue<V> Q;
    Q.push(s);

    while (!Q.empty()) {
        V v = Q.front(); Q.pop();
        S.push(v);

        for (V w : graph[v]) {
            if (d[w] < 0) {
                Q.push(w);
                d[w] = d[v] + 1;
            }
            if (d[w] == (d[v] + 1)) {
                sigma[w] += sigma[v];
                P[w].push_back(v);
            }
        }
    }

    while (!S.empty()) {
        V w = S.top(); S.pop();
        for (V v : P[w]) {
            delta[v] = delta[v] + ((sigma[v] / sigma[w]) * (1 + delta[w]));
        }

        if (w != s) {
            mutexes[w]->lock();
            BC[w] += delta[w];
            mutexes[w]->unlock();
        }
    }
}

int main(int argc, char* argv[]) {
    const int threads = std::stoi(std::string(argv[1]));
    const std::string inFile = std::string(argv[2]);
    const std::string outFile = std::string(argv[3]);

    Connectness BC;
    Graph graph;
    std::vector<V> vertices;
    std::unordered_map<V, std::unique_ptr<std::mutex>> mutexes;
    std::unordered_map<V, V> mapping;
    std::unordered_map<V, V> unmapping;

    read_input(inFile, BC, graph, vertices,
               mutexes, mapping, unmapping);

    
    pfor_each(vertices.begin(), vertices.end(),
              [&graph, &BC, &vertices, &mutexes, &mapping](V & v) {
                  Brandes(graph, BC, vertices, mapping[v], mutexes);
              },
              threads);

    print_output(outFile, BC, graph, mapping, vertices);

    return 0;
}
