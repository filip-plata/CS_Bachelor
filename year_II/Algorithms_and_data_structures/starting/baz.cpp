#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long MAX = 1000005;
const long long PosInf = 10000000000000000ll;
const long long NegInf = -10000000000000000ll;

long long sol[MAX];

int n, m;
vector<long long> cost;

/* Initialize to -Inifinity or +Infinity */

long long min_sufix_even[MAX];
long long max_prefix_even[MAX];
long long min_sufix_odd[MAX];
long long max_prefix_odd[MAX];

int main() {
  std::ios::sync_with_stdio(false);
  cin >> n;
  long long a;
  fill_n(min_sufix_even, MAX, PosInf);
  fill_n(min_sufix_odd, MAX, PosInf);
  fill_n(max_prefix_even, MAX, NegInf);
  fill_n(max_prefix_odd, MAX, NegInf);
  
  for (int i=0; i<n; i++) {
    cin >> a;
    cost.push_back(a);
    if ( a % 2 == 1) {
      max_prefix_odd[i+1] = max(max_prefix_odd[i], a);
      max_prefix_even[i+1] = max_prefix_even[i];
    }
    else {
      max_prefix_even[i+1] = max(max_prefix_even[i], a);
      max_prefix_odd[i+1] = max_prefix_odd[i];
    }
  }

  for (int i=n-1; i>=0; i--) {
    if (cost[i] % 2 == 1) {
      min_sufix_odd[i] = min(min_sufix_odd[i+1], cost[i]);
      min_sufix_even[i] = min_sufix_even[i+1];
    }
    else {
      min_sufix_even[i] = min(min_sufix_even[i+1], cost[i]);
      min_sufix_odd[i] = min_sufix_odd[i+1];
    }
  }

  long long max_sum = 0, temp_sum;

  for (int k=1; k <= n; k++) {
    int idx = n - k;
    max_sum += cost[idx];

    if (max_sum % 2 == 0) {
      long long pos_switch1 = max_sum - min_sufix_even[idx] + max_prefix_odd[idx];
      long long pos_switch2 = max_sum - min_sufix_odd[idx] + max_prefix_even[idx];
      temp_sum = max({(long long) -1, pos_switch1, pos_switch2});
    } else {
      temp_sum = max_sum;
    }

    sol[k] = temp_sum;
  }
  
  cin >> m;
  for (int i=0; i<m; i++) {
    int k; cin >> k;
    std::cout << sol[k] << std::endl;
  }
  return 0;
}
