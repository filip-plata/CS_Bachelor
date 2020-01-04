#include <cassert>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <set>

#include "../tree.h"

using namespace std;

bool LAZY_ALLOWED;

auto get_node(int l, bool is_random) {
  if (l <= 1) return Tree<int>::createValueNode(1);

  int left = is_random ? rand() % l : l - 1;
  int right = is_random ? rand() % l : l - 1;

  return Tree<int>::createValueNode(l, get_node(left, is_random), get_node(right, is_random));
}

Tree<int> get_tree(int l) {
  return Tree<int>(get_node(l, false));
}

Tree<int> get_random_tree(int l) {
  return Tree<int>(get_node(l, true));
}

Tree<int> get_line_tree(int l) {
  auto node = Tree<int>::createValueNode(1);
  auto em = Tree<int>::createEmptyNode();

  for (int i=2; i<=l; i++) {
    node = Tree<int>::createValueNode(i, node, em);
  }

  return Tree<int>(node);
}

void test_adding() {
  auto tree = get_line_tree(6);
  assert(tree.size() == 6);
  assert(tree.height() == 6);
}

void test_non_linear_tree() {
  auto tree = get_tree(6);
  assert(tree.size() == 63);
  assert(tree.height() == 6);
}

void test_apply() {
  auto tree = get_tree(5);
  int cnt = 0, sum = 0;
  tree.apply([&](int a)->void{cnt++; sum += a;}, Tree<int>::inorder);
  assert(cnt == 31);
  assert(sum == 57);
}

void test_accumulate() {
  auto tree = get_line_tree(5);
  assert(15 == tree.accumulate([](int a, int b)->int{return a + b; }, 0, Tree<int>::inorder));
}

void test_filter() {
  auto tree = get_random_tree(100);
  multiset<int> correct;
  auto filtered = tree.filter([&](int a)->bool{
      if (a % 2) {
          correct.insert(a);
          return true;
      } return false;
  });
  filtered.apply([&](int a)->void{assert(a % 2); correct.erase(correct.find(a));}, Tree<int>::inorder);
  assert(correct.empty() && "Drzewo ma za mało wartości!");
}

void test_map(int size) {
    multiset<int> elements;
    auto tree = get_tree(size);
    auto multiplied_tree = tree.map([&](int a)->int{elements.insert(a * 2); return a * 2;});
    multiplied_tree.apply([&](int a)->void{
        assert(elements.find(a) != elements.end());
        elements.erase(elements.find(a));
    }, Tree<int>::inorder);
    assert(elements.empty() && "Mapa nie zawiera odpowiednich elementów!");
}

void test_lazy_map() {
    multiset<int> elements, was_counted;
    LAZY_ALLOWED = false;
    auto tree = get_tree(10);
    tree.apply([&](int a)->void{was_counted.insert(a * 2);}, Tree<int>::inorder);
    auto multiplied_tree = tree.lazy_map([&](int a)->int{
        assert(LAZY_ALLOWED && "Wartości nie liczą się leniwie!"); elements.insert(a * 4); return a * 2;
    });
    multiplied_tree = multiplied_tree.lazy_map([&](int a)->int{
        assert(LAZY_ALLOWED);
        assert(was_counted.find(a) != was_counted.end() && "Lazy map powinno liczyć wartości tylko raz!");
        was_counted.erase(was_counted.find(a));
        return a * 2;
    });

    LAZY_ALLOWED = true;

    multiplied_tree.apply([&](int a)->void {
        assert(elements.find(a) != elements.end());
        elements.erase(elements.find(a));
    }, Tree<int>::inorder);

    multiplied_tree.apply([&](int a)->void {
        // jeśli lazy map jest źle zrobione tu wartość będzie liczona drugi raz
    }, Tree<int>::inorder);
    assert(elements.empty());
}

void test_map_type_change() {
    Tree<int> tree = get_tree(10);
    Tree<double> double_tree = tree.map([](int a)->double{return a + 0.2;});
    double_tree.apply([](double a)->void{assert(a - (int)a > 0.1 && "Drzewo źle mapuje inty na double");},
                      Tree<double>::preorder);
    tree = double_tree.map([](double a)->int{return (int)a;});
    tree.apply([](int a)->void{}, Tree<int>::preorder);
}

void test_lazy_map_type_change() {
    Tree<int> tree = get_tree(10);
    
    int sum = tree.accumulate([](int a, int b)-> int{return a + b;}, 0, Tree<int>::inorder);
    
    Tree<double> double_tree = tree.lazy_map([](int a)->double{return a + 0.2;});
    
    double_tree.apply([](double a)->void{assert(a - (int)a > 0.1 && "Drzewo źle mapuje inty na double");},
                      Tree<double>::preorder);

    LAZY_ALLOWED = false;
    Tree<unsigned int> unsigned_tree = tree.lazy_map([](int a)->unsigned int{return (unsigned int) a;});
    double_tree = unsigned_tree.lazy_map([](int a)->double{return a + 0.2;});
    Tree<string> string_tree = double_tree.lazy_map([](double a)->string{return to_string(a);});
    LAZY_ALLOWED = true;
    assert(sum == string_tree.accumulate([](int a, string b)-> int{return a + stoi(b);}, 0, Tree<string>::inorder));
    
}

auto make_preorder_tree(int level) {
    static int cnt = 1;
    if (level == 0) return Tree<int>::createEmptyNode();
    int my_num = cnt++;
    auto left = make_preorder_tree(level - 1);
    return Tree<int>::createValueNode(my_num, left, make_preorder_tree(level - 1));
}

auto make_inorder_tree(int level) {
    static int cnt = 1;
    if (level == 0) return Tree<int>::createEmptyNode();
    auto left = make_inorder_tree(level - 1);
    int my_num = cnt++;
    return Tree<int>::createValueNode(my_num, left, make_inorder_tree(level - 1));
}

auto make_postorder_tree(int level) {
    static int cnt = 1;
    if (level == 0) return Tree<int>::createEmptyNode();
    auto left = make_postorder_tree(level - 1);
    auto right = make_postorder_tree(level - 1);
    int my_num = cnt++;
    return Tree<int>::createValueNode(my_num, left, right);
}

void test_order(Tree<int> tree, auto order) {
    int last = 0;
    tree.apply([&](int a)->void{assert(a > last); last = a;}, order);
}

void test_preorder() {
    printf("test_preorder\n");
    test_order(Tree<int>(make_preorder_tree(10)), Tree<int>::preorder);
}

void test_inorder() {
    printf("test_inorder\n");
    test_order(Tree<int>(make_inorder_tree(10)), Tree<int>::inorder);
}

void test_postorder() {
    printf("test_postorder\n");
    test_order(Tree<int>(make_postorder_tree(10)), Tree<int>::postorder);
}

void test_empty() {
    auto tree = Tree<int>();
    assert(tree.size() == 0);
    assert(tree.height() == 0);

    int cnt = 0;
    tree.apply([&](int a)->void{cnt++;}, Tree<int>::inorder);
    assert(!cnt);
    assert(tree.accumulate([](int a, int b)->int{return a + b;}, 0, Tree<int>::postorder) == 0);

    Tree<double> mapped = tree.map([](int a)->double{return a;});
    assert(mapped.size() == 0);
}

void test_is_bst() {
    auto empty = Tree<int>();
    assert(empty.is_bst());

    Tree<int> tree = make_inorder_tree(10);
    assert(tree.is_bst());
    assert(!(Tree<int>(make_preorder_tree(10)).is_bst()));
}

int main() {
    srand(time(0));
    test_empty();
    test_adding();
    test_non_linear_tree();
    test_apply();
    test_filter();
    test_accumulate();
    test_map(10);
    //test_map(20);  // <- robi milion nodów
    test_lazy_map();
    test_map_type_change();
    test_lazy_map_type_change();
    test_preorder();
    test_postorder();
    test_inorder();
    test_is_bst();
}
