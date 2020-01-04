#include <iostream>
#include <functional>
#include <sstream>
#include <string>
#include "../tree.h"

int main(int argc, const char * argv[]) {
    auto two = Tree<int>::createValueNode(2);
    auto one = Tree<int>::createValueNode(1, Tree<int>::createEmptyNode(), two);
    auto four = Tree<int>::createValueNode(4);
    auto five = Tree<int>::createValueNode(5,
                                           Tree<int>::createValueNode(4),
                                           Tree<int>::createEmptyNode());
    auto root = Tree<int>::createValueNode(3, one, five);
    Tree<int> tree(root);

    tree.print(Tree<int>::inorder);
    tree.print(Tree<int>::preorder);
    tree.print(Tree<int>::postorder);

    std::cout << "size: " << tree.size() << std::endl;
    std::cout << "height: " << tree.height() << std::endl;
    std::cout << "bst: " << tree.is_bst() << std::endl;

    std::cout << "custom print: ";
    tree.apply([](int &e){std::cout << e << ":";}, Tree<int>::inorder);
    std::cout << std::endl;

    int x = 2;
    
    Tree<int> minus_x = tree.map([&](int e)->int{return e - x;});
    Tree<int> lazy_minus_x = tree.lazy_map([&](int e)->int{return e - x;});
    
    x = 4;
    std::cout << "-2: ";
    minus_x.print();
    std::cout << "-4: ";
    lazy_minus_x.print();

    Tree<int> odd = minus_x.filter([](int e)->bool{return (e & 1) == 1;});
    std::cout << "odd: ";
    odd.print();

    int sum1 = tree.accumulate(std::plus<int>(), 0, Tree<int>::inorder);
    std::cout << "sum: " << sum1 << std::endl;

    int sum2 = tree.fold([](int val, int l, int r)->int{return val + l + r;}, 0);
    std::cout << "sum: " << sum2 << std::endl;
}
