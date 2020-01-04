#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <iterator>

using namespace std;

long long calc(vector<pair<int, int>> & v) {
    long long res = 0;
    int n = v.size();
    sort(v.begin(), v.end());

    vector<int> starts(n);

    for (int i=0; i<n; i++) {
	starts[i] = v[i].first;
    }

    for (int i=0; i<n; i++) {
	int end = v[i].second;
	res += distance(starts.begin()+i, upper_bound(starts.begin()+i, starts.end(), end));
	res--;
    }
    
    
    return res;
}


int main() {
    std::ios::sync_with_stdio(false);
    int n;
    map<int, vector<pair<int, int>>> d;
    long long res = 0;
    cin >> n;

    for (int i=0; i<n; i++) {
	int x, y1, y2;
	cin >> x >> y1 >> y2;

	if (d.find(x) == d.end())
	    d[x] = vector<pair<int,int>>();
	d[x].push_back(make_pair(y1, y2));
    }

    for (auto & v : d)
	res += calc(v.second);

    cout << res << endl;
    
    return 0;
}
