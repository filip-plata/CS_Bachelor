#include <algorithm>
#include <set>
#include <iostream>
#include <vector>


using namespace std;


int main() {
    int n;
    cin >> n;
    vector<int> types(n);
    set<int> current;
    int result = 0;
    for (int i=0; i<n; i++)
	cin >> types[i];

    for (int i=0; i<n; i++) {
	if (current.find(types[i]) != current.end()) {
	    result++;
	    current.clear();
	} else {
	    current.insert(types[i]);
	}
    }

    cout << result << endl;
    
    return 0;
}
