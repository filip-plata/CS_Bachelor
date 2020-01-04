#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>
#include <future>
#include "../tree.h"


int main() {
    auto one = Tree<int>::createValueNode(1);
    auto two = Tree<int>::createValueNode(2);
    auto three = Tree<int>::createValueNode(3, one, two);
    auto four = Tree<int>::createValueNode(4);
    auto five = Tree<int>::createValueNode(5, four, three);

    Tree<int> tree(five);

    std::function<int(int)> slow_identity =
        [] (int val) {
        std::this_thread::sleep_for(std::chrono::seconds(100));
        return val;
    };

    auto future_tree_first = std::async(std::launch::async, [&]() {
            tree.lazy_map(slow_identity);
            // nie chcemy odpalać copy-constructor
            return 5;
        });

    auto status = future_tree_first.wait_for(std::chrono::seconds(1));

    if (status == std::future_status::ready) {
        auto tree_first = tree.lazy_map(slow_identity);
        auto future_tree_second = std::async(std::launch::async, [&](){
            tree_first.lazy_map(slow_identity);
            // nie chcemy odpalać copy-constructor
            return 5;
        });

        status = future_tree_second.wait_for(std::chrono::seconds(1));

        if (status != std::future_status::ready) {
            std::cerr << "LAZY MAP IS NOT LAZY AT DEPTH 2 COPY" << std::endl;         
        } else {
            std::cerr << "LAZY IT IS" << std::endl;
        }
        
    } else {
        std::cerr << "LAZY MAP IS NOT LAZY" << std::endl;
        return 1;
    }

    return 0;
}
