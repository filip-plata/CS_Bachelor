#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <functional>

class Tree {
public:
    static const std::size_t constexpr MAX_DEPTH = 19;

    class TreeNode {
    public:
        int left, right, parent, bestTurn;
        int upLinks[MAX_DEPTH];
        int downLinks[MAX_DEPTH];
        int value, height, pos;

        TreeNode(int _value)
                : left(-1), right(-1), parent(-1),
                  bestTurn(-1), value(_value), height(0), pos(0) {
            for (int i = 0; i < MAX_DEPTH; i++) {
                upLinks[i] = -1;
                downLinks[i] = -1;
            }
        }

        TreeNode() : TreeNode(-1) {}

        int getBrother() {
            if (this->parent == -1)
                return -1;
            if (this->value == nodes[this->parent].left)
                return nodes[this->parent].right;
            return nodes[this->parent].left;
        }
    };

    static std::vector<TreeNode> nodes;

    typedef int (&Links)[MAX_DEPTH];
    typedef std::function<Links(int )> LinksGet;

    std::size_t N;

    int _goDirection(TreeNode &start, int d, LinksGet links_function) {
        // Assume its possible to go d in direction
        int num = start.value;
        if (d == 0)
            return num;
        int pos = 0;
        while (d > 0) {
            if (d % 2 == 1)
                num = links_function(num)[pos];
            d >>= 1;
            pos++;
        }
        return num;
    }

    int _goUp(TreeNode &start, int d) {
        LinksGet upLinks = [](int x) -> Links { return nodes[x].upLinks; };
        return _goDirection(start, d, upLinks);
    }

    int _goDown(TreeNode &start, int d) {
        LinksGet downLinks = [](int x) -> Links { return nodes[x].downLinks; };
        return _goDirection(start, d, downLinks);
    }

    int _findNode(TreeNode &start, int d) {
        if (start.height >= d)
            return _goDown(start, d);
        if (start.bestTurn == -1)
            return -1;
        TreeNode turn = nodes[start.bestTurn];

        if (start.pos - turn.pos + 1 >= d)
            return _goUp(start, d);

        d = d - (start.pos - turn.pos + 2);

        if (d <= turn.height)
            return _goDown(turn, d);

        return -1;
    }

    void visit_postorder(int node, const std::function<void(TreeNode &)> &fun) {
        if (node == -1)
            return;
        TreeNode &v = nodes[node];
        visit_postorder(v.left, fun);
        visit_postorder(v.right, fun);

        fun(v);
    }

    void visit_preorder(int node, const std::function<void(TreeNode &)> &fun) {
        if (node == -1)
            return;
        TreeNode &v = nodes[node];
        fun(v);
        visit_preorder(v.left, fun);
        visit_preorder(v.right, fun);
    }

    void calculate_data() {
        visit_postorder(1, [](TreeNode &v) -> void {
            int left_h = v.left != -1 ? nodes[v.left].height : 0;
            int right_h = v.right != -1 ? nodes[v.right].height : 0;
            if (v.left != -1 || v.right != -1)
                v.height = 1 + std::max(left_h, right_h);

            if (left_h >= right_h && v.left != -1)
                v.downLinks[0] = v.left;
            else if (v.right != -1)
                v.downLinks[0] = v.right;
        });

        visit_preorder(1, [](TreeNode &v) -> void {
            if (v.parent != -1)
                v.pos = nodes[v.parent].pos + 1;
        });

        visit_preorder(1, [](TreeNode &v) -> void {
            int brother = v.getBrother(), parent = v.parent;

            if (brother == -1 && parent == -1)
                return;
            if (brother == -1) {
                v.bestTurn = nodes[parent].bestTurn;
                return;
            }

            if (parent == -1 || nodes[parent].bestTurn == -1) {
                v.bestTurn = brother;
                return;
            }

            if (nodes[brother].height >=
                v.pos - nodes[nodes[parent].bestTurn].pos + nodes[nodes[parent].bestTurn].height)
                v.bestTurn = brother;
            else
                v.bestTurn = nodes[parent].bestTurn;
        });
    }

    void setup_links() {
        for (int i = 1; i < MAX_DEPTH; i++) {
            for (int j = 1; j < N; j++) {
                int inter = nodes[j].upLinks[i - 1];
                if (inter != -1)
                    nodes[j].upLinks[i] = nodes[inter].upLinks[i - 1];

                inter = nodes[j].downLinks[i - 1];
                if (inter != -1)
                    nodes[j].downLinks[i] = nodes[inter].downLinks[i - 1];
            }
        }
    }

public:
    Tree(std::size_t size)
            : N(size + 1) {
        nodes = std::vector<TreeNode>(size + 1, TreeNode());
        for (int i = 1; i < N; i++)
            nodes[i].value = i;
    }

    void setChildren(int i, int a, int b) {
        if (a >= 1) {
            nodes[i].left = a;
            nodes[a].parent = i;
        }
        if (b >= 1) {
            nodes[b].parent = i;
            nodes[i].right = b;
        }
    }

    void finalize() {
        calculate_data();
        for (int i = 2; i < N; i++)
            nodes[i].upLinks[0] = nodes[i].parent;
        setup_links();
    }

    int findNode(int a, int d) {
        return _findNode(nodes[a], d);
    }
};

std::vector<Tree::TreeNode> Tree::nodes;


int main() {
    std::ios::sync_with_stdio(false);
    int n, a, b, m;
    std::cin >> n;

    Tree t((std::size_t) n);

    for (int i = 1; i <= n; i++) {
        std::cin >> a >> b;
        t.setChildren(i, a, b);
    }

    t.finalize();

    std::cin >> m;

    while (m--) {
        std::cin >> a >> b;
        std::cout << t.findNode(a, b) << '\n';
    }

    std::cout << std::flush;

    return 0;
}
