#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

int main() {
  int n, m, a;
  cin >> n >> m;
  vector<vector<bool>> matches(n, vector<bool>(m));

  for (int i=0; i<m; i++)
    for (int j=0; j<n; j++) {
      cin >> a;
      matches[--a][i] = j < n/2;
    }

  sort(matches.begin(), matches.end());
  cout << ((unique(matches.begin(), matches.end()) ==
	    matches.end()) ? "TAK" : "NIE") << endl;
  return 0;
}
