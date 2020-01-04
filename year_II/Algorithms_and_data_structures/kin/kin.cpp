#include <iostream>
#include <vector>
#include <algorithm>

typedef long long ll;
typedef unsigned long long ull;

const ull P_MOD = 1000000000ll;

class SegmentTree {
    /* Insert and query referenced from:
    *  http://codeforces.com/blog/entry/18051
    *  for a very terse and efficient
    *  implementation. Very terse. */
private:
  ull N;
  ull range;
  std::vector<ull> _data;

  ull next_pow(ull n) {
    ull result = 1;

    while (n > 0) {
      n >>= 1;
      result <<= 1;
    }

    return result;
  }
public:
  SegmentTree(ull n)
    : N(next_pow(n)), range(n), _data(2*N, 0) {
  }
  
  ull sumRange(int l, int r) {
    if (r <= l || r > _data.size() || l < 0)
      return 0;
    
    ull res = 0;
    for (l += N, r += N; l < r; l >>= 1, r >>= 1) {
      if (l&1) res += _data[l++];
      if (r&1) res += _data[--r];
      res %= P_MOD;
    }
    return res;
  }
  
  void insert(int idx, ull value) {
    if (idx < 0 || idx >= range)
      return;
    for (_data[idx += N] = value; idx /= 2; )
      _data[idx] = (_data[idx * 2] + _data[idx * 2 + 1]) % P_MOD; 
  }
};

int main() {
  std::size_t n, k;
  std::cin >> n >> k;
  std::vector<SegmentTree> trees(k, SegmentTree(n));
  std::vector<ull> data(n);

  for (std::size_t i=0; i<n; i++)
    std::cin >> data[i];

  for (auto x : data) {
    ull idx = x - 1;
    trees[0].insert(idx, 1);
    
    for (int j=1; j<k; j++) {
      ull valid_inversions = trees[j-1].sumRange(idx+1, n);
      trees[j].insert(idx, valid_inversions);
    }
  }

  std::cout << trees[k-1].sumRange(0, n) << std::endl;
  
  return 0;
}
