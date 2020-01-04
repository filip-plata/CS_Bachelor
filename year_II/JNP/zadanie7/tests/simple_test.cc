#include <iostream>
#include <cassert>
#include "../tree.h"


int main() {
    auto one = Tree<int>::createValueNode(1);
    auto two = Tree<int>::createValueNode(2);
    auto three = Tree<int>::createValueNode(3, one, two);
    auto four = Tree<int>::createValueNode(4);
    auto five = Tree<int>::createValueNode(5, four, three);

    auto help = Tree<int>::createValueNode(2, one, Tree<int>::createValueNode(3));

    std::function<bool(int)> pred = [](int x) { return x == 1; };

    Tree<int> bst(help);

    assert(bst.is_bst());
    Tree<int> filtered = bst.filter(pred);
    assert(filtered.size() == 1);
    assert(filtered.height() == 1);
    assert(filtered.accumulate(std::plus<int>(), 0, Tree<int>::inorder) == 1);

    Tree<int> tree(five);

    assert(tree.height() == 3);
    assert(tree.size() == 5);
    assert(!tree.is_bst());

    tree.print();

    std::function<std::string(int)> xd = [](int x) { return std::string("xd"); };
    std::function<int(int)> addOne = [](int x) { return x + 1; };

    Tree<std::string> xddd = tree.map(xd);
    Tree<int> tree_one = tree.map(addOne);

    tree_one.print();

    assert(tree_one.height() == 3);
    assert(tree_one.size() == 5);
    
    Tree<int> tree_two = tree_one.lazy_map(addOne);

    std::cout << tree_two.height() << std::endl;
    assert(tree_two.height() == 3);
    assert(tree_two.size() == 5);

    tree_two.print(Tree<int>::postorder);

    assert(tree_two.accumulate(std::plus<int>(), 0, Tree<int>::inorder) == 25);

    assert(tree.height() == 3);
    assert(tree.size() == 5);
    assert(!tree.is_bst());

    //tree.filter(pred);
    
    return 0;
}
