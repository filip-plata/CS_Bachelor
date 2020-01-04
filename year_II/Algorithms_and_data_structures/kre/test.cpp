#include <random>
#include <cassert>
#include <algorithm>
#include <iostream>
#include "segment_tree.h"

const int TEST_CASES = 10;
const int TEST_DEPTH = 10000;
const int RANGE = 100;
const ll middle = 1000000000ll;


using namespace std;

void test_random(int vector_size) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(-RANGE, RANGE);

    std::uniform_int_distribution<int> v_range(0, vector_size-1);    

    std::vector<ll> randomized_v(vector_size);

    for (int i=0; i < vector_size; i++)
		randomized_v[i] = middle + uni(rng);

    SegmentTree tree(randomized_v);

    for (int i=0; i < TEST_DEPTH; i++) {
		int a = v_range(rng);
		int b = v_range(rng);
		int d = uni(rng);

		if (b < a)
			swap(a, b);
		b++;


		tree.addToRange(a, b, d);
        for (int i=a; i<b; i++)
            randomized_v[i] += d;

        ll tree_max = tree.getMax(a, b);
        ll tree_min = tree.getMin(a, b);
        ll vector_max = *max_element(randomized_v.begin()+a, randomized_v.begin()+b);
        ll vector_min = *min_element(randomized_v.begin()+a, randomized_v.begin()+b);

        assert(tree_max == vector_max);
        assert(tree_min == vector_min);
        assert(tree.getValue(a) == randomized_v[a]);

        ll increase = tree.queryRange(a, b, d);

        for (int i=a; i<b; i++)
            randomized_v[i] += d;

        ll vector_increase = 0;

        for (int i=1; i<randomized_v.size(); i++)
            if (randomized_v[i-1] < randomized_v[i])
                vector_increase++;

        assert(vector_increase == increase);
    }
}

int main() {
    std::vector<int> vector_size{1, 5, 10, 8, 16, 1000, 512, 2048, 2047, 100};
    for (int t=0; t < TEST_CASES; t++)
        test_random(vector_size[t]);
}
