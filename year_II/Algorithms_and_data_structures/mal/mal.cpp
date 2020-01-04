#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

class SegmentTree {
private:
    int N;
    static int constexpr MAX_SIZE = 1048576;

    struct Node {
	int sum;
	int size;
	int lazy;

	Node() : sum(0), size(1), lazy(-1) {}
    };

    void push_lazy(int root, int l, int r) {
	if (_data[root].lazy == -1)
	    return;
	_data[root].sum = _data[root].lazy * _data[root].size;
	if (l != r) {
	    _data[root*2].lazy = _data[root].lazy;
	    _data[root*2 + 1].lazy = _data[root].lazy;
	}
	
	_data[root].lazy = -1;
    }

    void query_tree_set_value(int root, int l, int r, int i, int j, int value) {
	//std::cerr << "VALUE: " << root << " " << l << " " << r << " " << i << " " << j << " " << value << std::endl;
	push_lazy(root, l, r);
	if (l > j || r < i || l > r)
	    return;

	if (i <= l && r <= j) {
	    _data[root].lazy = value;
	    push_lazy(root, l, r);
	    return;
	}

	query_tree_set_value(2*root, l, (l+r)/2, i, j, value);
	query_tree_set_value(2*root + 1, (l+r)/2 + 1, r, i, j, value);

	_data[root].sum = _data[root*2].sum + _data[root*2+1].sum;
    }

    int query_tree_sum(int root, int l, int r, int i, int j) {
	//std::cerr << "SUM: " << root << " " << l << " " << r << " " << i << " " << j << std::endl;
	if (l > j || r < i || l > r)
	    return 0;
	push_lazy(root, l, r);

	if (i <= l && r <= j)
	    return _data[root].sum;

	int res1 = query_tree_sum(2*root, l, (l+r)/2, i, j);
	int res2 = query_tree_sum(2*root + 1, (l+r)/2 + 1, r, i, j);

	return res1 + res2;
    }

    std::vector<Node> _data;

    int next_pow(int n) {
        int x = (int)(ceil(log2(n)))+1;
        return (1 << x);
    }
public:
    SegmentTree(int n)
	: N(next_pow(n)), _data(MAX_SIZE * 2 + 7) {
	for (int i=N-1; i>0; i--) {
	    _data[i].size = 2 * _data[2 * i].size;
	}
    }

    int queryRange(int l, int r, int value) {
	query_tree_set_value(1, 0, N-1, l, r, value);
	return query_tree_sum(1, 0, N-1, 0, N-1);
    }
};


int main() {
    int n, m, a, b;
    char C;

    std::cin >> n >> m;

    SegmentTree tree(n);

    while (m--) {
	std::cin >> a >> b >> C;
	std::cout << tree.queryRange(--a, --b, (C == 'C') ? 0 : 1) << std::endl;
    }

    return 0;
}
