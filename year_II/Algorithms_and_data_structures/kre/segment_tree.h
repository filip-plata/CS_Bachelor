#ifndef KRE_SEGMENT_TREE_H
#define KRE_SEGMENT_TREE_H

#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

typedef long long ll;
typedef unsigned long long ull;

class SegmentTree {
private:
    class TreeNode {
    public:
        ll max;
        ll min;
        ll lazy_value;

        TreeNode() : max(-INF), min(INF), lazy_value(0) {}

        TreeNode(ll _value)
                : max(_value), min(_value),
                  lazy_value(0){}

        TreeNode(TreeNode & node1, TreeNode & node2)
                : max(std::max(node1.max, node2.max)),
                  min(std::min(node1.min, node2.min)), lazy_value(0) {
        }
    };

    int N;
    int increases;
    std::vector<TreeNode> _data;
    std::vector<bool> _increase;

    static const ll MAX_VALUE = 2000000000;
    static const ll MIN_VALUE = 0;
    static const ll INF = 1000000000000000ll;

    void update_increase(int idx) {
        if (idx <= 0 || idx > _increase.size())
            return;
        bool new_value = getValue(idx-1) < getValue(idx);
        bool old_value = _increase[idx-1];

        if (old_value == new_value)
            return;

        int change = (new_value && !old_value) ? 1 : -1;

        increases += change;
        _increase[idx-1] = new_value;
    }

    void push_lazy(int node, int a, int b) {
        if (_data[node].lazy_value == 0)
            return;
        _data[node].max += _data[node].lazy_value;
        _data[node].min += _data[node].lazy_value;
        if (a != b) {
            _data[2*node].lazy_value += _data[node].lazy_value;
            _data[2*node + 1].lazy_value += _data[node].lazy_value;
        }
        _data[node].lazy_value = 0;
    }

    void update_tree(int node, int a, int b, int i, int j, ll v) {
        push_lazy(node, a, b);

        if (a > b || b < i || a > j)
            return;

        if (a >= i && b <= j) {
            _data[node].lazy_value += v;
            push_lazy(node, a, b);
            return;
        }

        update_tree(node*2, a, (a+b)/2, i, j, v);
        update_tree(node*2+1, (a+b)/2 + 1, b, i, j, v);

        _data[node].max = std::max(_data[node*2].max, _data[node*2+1].max);
        _data[node].min = std::min(_data[node*2].min, _data[node*2+1].min);
    }

    ll query_tree_max(int node, int a, int b, int i, int j) {
        if (a > b || a > j || b < i)
            return -INF;
        push_lazy(node, a, b);

        if (a >= i && b <= j)
            return _data[node].max;

        ll q1 = query_tree_max(node * 2, a, (a + b) / 2, i, j);
        ll q2 = query_tree_max(node * 2 + 1, 1 + (a + b) / 2, b, i, j);

        return std::max(q1, q2);
    }

    ll query_tree_min(int node, int a, int b, int i, int j) {
        if (a > b || a > j || b < i)
            return INF;
        push_lazy(node, a, b);

        if (a >= i && b <= j)
            return _data[node].min;

        ll q1 = query_tree_min(node * 2, a, (a + b) / 2, i, j);
        ll q2 = query_tree_min(node * 2 + 1, 1 + (a + b) / 2, b, i, j);

        return std::min(q1, q2);
    }

    int next_pow(ull n) {
        int x = (int)(ceil(log2(n)))+1;
        return (1 << x);
    }
public:
    SegmentTree(std::vector<ll> &data)
            : N(next_pow(data.size())), increases(0), _data(N * 2 + 7),
              _increase(data.size() - 1) {
        for (ull i = 0; i<data.size(); i++)
            _data[N+i] = TreeNode(data[i]);

        for (int i = N-1; i>0; i--)
            _data[i] = TreeNode(_data[2*i], _data[2*i+1]);

        for (ull i=1; i < data.size(); i++)
            _increase[i-1] = data[i-1] < data[i];

        increases = std::count(_increase.begin(), _increase.end(), true);
    };

    ll queryRange(int l, int r, ll d) {
        ll max = getMax(l, r);
        ll min = getMin(l, r);

        if (d + max > MAX_VALUE || d+min < MIN_VALUE)
            return -1;

        addToRange(l, r, d);

        update_increase(r);
        update_increase(l);

        return increases;
    }

    void addToRange(int l, int r, ll value) {
	    update_tree(1, 0, N-1, l, r-1, value);
    }

    ll getValue(ll idx) {
        idx += N;
        ll res = _data[idx].max + _data[idx].lazy_value;
        while ((idx /= 2))
            res += _data[idx].lazy_value;
        return res;
    }

    ll getMax(int l, int r) {
	    return query_tree_max(1, 0, N-1, l, r-1);
    }

    ll getMin(int l, int r) {
        return query_tree_min(1, 0, N-1, l, r-1);
    }
};

#endif //KRE_SEGMENT_TREE_H
