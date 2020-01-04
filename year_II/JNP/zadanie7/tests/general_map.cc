#include <iostream>
#include <cassert>
#include <string>
#include "../tree.h"


int main() {
    auto one = Tree<std::string>::createValueNode("korba!");
    auto two = Tree<std::string>::createValueNode("jazda!", one,
                    Tree<std::string>::createEmptyNode());
    auto three = Tree<std::string>::createValueNode("amok!");
    auto four = Tree<std::string>::createValueNode("chłosta!");
    auto five = Tree<std::string>::createValueNode("dopał!");
    auto six = Tree<std::string>::createValueNode("rwanie!", three, two);
    auto seven = Tree<std::string>::createValueNode("faza!", five, four);
    auto eight = Tree<std::string>::createValueNode("zgon!", seven, six);

    Tree<std::string> tree(eight);

    assert(tree.size() == 8);
    assert(tree.height() == 4);
    assert(!tree.is_bst());

    tree.print(Tree<std::string>::preorder);

    std::function<int(std::string)> len = [](std::string str) {
        return str.length();
    };

    Tree<int> tree_length = tree.map(len);

    assert(tree_length.size() == 8);
    assert(tree_length.height() == 4);
    assert(!tree_length.is_bst());

    int sum1 = tree_length.accumulate(std::plus<int>(), 0, Tree<int>::inorder);

    std::function<int(int, std::string)> add_len =
        [](int res, std::string str) {
        return res + str.length();
    };
    int sum2 = tree.accumulate(add_len, 0, Tree<std::string>::inorder);

    assert(sum1 == sum2);
    
    Tree<std::string> tree_filtered = tree.filter([](std::string str) {
            return str != std::string("korba!");
        });

    int sum3 = tree_filtered.accumulate(add_len, 0, Tree<std::string>::postorder);

    assert(tree_filtered.size() == 7);
    assert(tree_filtered.height() == 3);
    assert(!tree_filtered.is_bst());
    assert(sum3 == (sum1 - 6));
    
    return 0;
}
