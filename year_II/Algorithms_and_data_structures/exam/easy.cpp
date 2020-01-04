#include <algorithm>
#include <set>
#include <vector>
#include <queue>
#include <map>
#include <iostream>
#include <cassert>


using namespace std;

typedef vector<vector<int>> Graph;

static constexpr long long MAX = 100005;
static constexpr long long INF = 1000000000000000l;

long long res[MAX][4][2];

long long calc_not_taking(int k) {
    long long result = 0;
    for (int i=0; i<4; i++) {
	for(int j=0; j<2; j++) {
	    result = max(result, res[k][i][j]);
	}
    }
    return result;
}

int main() {
    std::ios::sync_with_stdio(false);
    /*
    for (int i=0; i<MAX; i++) {
	for (int u=0; u<4; u++) {
	    for (int v=0; v<2; v++) {
		res[i][u][v] = -INF;
	    }
	}
    }
    */

    int n;
    cin >> n;
    vector<int> d(n);

    for (int i=0; i<n; i++)
	cin >> d[i];

    for (int i=0; i<n; i++) {
	for (int j=0; j<2; j++) {
	    res[i+1][0][j] = max({res[i][0][j], res[i][1][j], res[i][2][j], res[i][3][j]});
	    res[i+1][1][j] = max({res[i][0][j] + d[i]});
	    res[i+1][2][j] = max({res[i][1][j] + d[i]});
	}
	res[i+1][3][0] = -INF;
	res[i+1][3][1] = max({res[i][2][0] + d[i]});
    }

    long long result = calc_not_taking(n);

    cout << result << endl;
    
    
    return 0;
}
