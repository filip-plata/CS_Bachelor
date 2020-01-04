#include <map>
#include <iostream>
#include <algorithm>


using namespace std;

int n, q;

int main() {
    map<int, int> m;
    cin >> n >> q;
    for (int i=0; i<n; i++) {
	int p;
	cin >> p;
	if (m.find(p) == m.end())
	    m[p] = 1;
	else
	    m[p] += 1;
    }

    while (q--) {
	int l, r, d, result = 0;
	cin >> l >> r >> d;
	auto it = m.lower_bound(l);
	auto end = m.upper_bound(r);

	while (it != end) {
	    if (it->first != d) {
		result += it->second;
		it = m.erase(it);
	    } else {
		++it;
	    }
	}

	if (m.find(d) == m.end())
	    m[d] = result;
	else
	    m[d] += result;

	cout << result << endl;
    }
    
    return 0;
}
