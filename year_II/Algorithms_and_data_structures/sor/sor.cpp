#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

const int MAX = 1005;
const long P = 1000000000ll;

int n = 0;
long t[MAX];
long result = 0;
// first ending in number, second how much
vector<pair<long, long>> res[MAX][MAX];


int main() {
  cin >> n;

  for (int i=0; i<n; i++)
    cin >> t[i];

  for (int len=1; len <= n; len++) {
    for (int i=0; i<=n-len; i++) {
      // [i..i+len-1]
      if (len == 1) {
	res[i][i] = vector<pair<long, long>>{make_pair(t[i], 1)};
	continue;
      }
      long right = 0, left = 0;

      for (auto ans : res[i][i+len-2]) {
	if (ans.first < t[i+len-1])
	  right += ans.second;
      }

      for (auto ans : res[i+1][i+len-1]) {
	if (ans.first > t[i])
	  left += ans.second;
      }

      right %= P;
      left %= P;
      
      res[i][i+len-1].push_back(make_pair(t[i+len-1], right));
      res[i][i+len-1].push_back(make_pair(t[i], left));
    }
  }

  for (auto ans : res[0][n-1])
    result += ans.second;

  result %= P;
  cout << result << endl;
  return 0;
}
