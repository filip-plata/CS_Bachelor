#ifndef TREE_H
#define TREE_H

#include <functional>
#include <algorithm>
#include <memory>
#include <vector>
#include <utility>
#include <iostream>
#include <type_traits>


template <typename T>
class Tree {
private:
    /* Template chciałby być przyjacielem samego siebie
       w funkcji map. */
    template <typename XD>
    friend class Tree;
    
    class Node {
    private:
        /* Leniwość wartości osiągana jest przez defaultowe
           przechowywanie nullptr oraz funkcji T(void). */
        std::shared_ptr<T> value;
        std::function<T(void)> ret;

        void init_val() {
            // ret == nullptr dla EmptyNode
            if (ret) {
                value = std::make_shared<T>(ret());
                ret = [this]() { return *(this->value); };
            }
        }
    public:
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        
        Node() {};
        template <typename F>
        Node(Node left_, Node right_, F fun)
            : left(std::make_shared<Node>(left_)),
              right(std::make_shared<Node>(right_)),
              ret(fun){}
        Node(T &val,
             std::shared_ptr<Node> left_,
             std::shared_ptr<Node> right_)
            : left(left_),
              right(right_) {
            ret = [val]() {return val; };
            init_val();
        }
        Node(T &val, Node left_, Node right_)
            : left(std::make_shared<Node>(left_)),
              right(std::make_shared<Node>(right_)) {
            ret = [val]() {return val; };
            init_val();
        }

        // tutaj możnaby rozważyć klasę abastrakcyjną i dwie podklasy
        // zamiast tej metody
        // jednak tej funkcji potrzebuję tylko w fold_ i traversal
        // i to świadoma decyzja, żeby zrobić jedno linijkową metodę
        // zamaist dwóch kolejnych klas.
        bool hasValue() {
            return bool(ret);
        }
        
        std::weak_ptr<T> getValue() {
            /* Jeśli wartość jest nullem, to ją liczymy */
            if (!value)
                init_val();
            return std::weak_ptr<T>(value);
        }
    };
    
    typedef std::function<void(std::weak_ptr<Node>,
                               std::function<void(T&)>&)> Traversal_t;

    /* Parametryzowane zwracanym typem, żeby lazy_map
       mogło operować na wskaźnikach na nody zamiast na wartościach */
    template <typename R, typename E>
    R fold_(std::function<R(E, R, R)> &f, R init,
            std::weak_ptr<Node> node,
            std::function<E(std::weak_ptr<Node>)> &eval) const {
        if (!node.lock()->hasValue()) {
            return init;
        } else {
            R left = fold_(f, init,
                           std::weak_ptr<Node>(node.lock()->left), eval);
            R right = fold_(f, init,
                            std::weak_ptr<Node>(node.lock()->right), eval);
            return f(eval(node), left, right);
        }
    }
    
    std::shared_ptr<Node> root;
    static T id(T val) { return val; };
public:
    Tree() : root(std::make_shared<Node>(createEmptyNode()) ){}
    Tree(Node &root_) : root(std::make_shared<Node>(root_)) {}
    Tree(Node &&root_) : root(std::make_shared<Node>(std::move(root_))) {}
    Tree(Tree const& tree) {
        this->root = tree.map(Tree<T>::id).root;
    }

    /*
     * Funkcje poniżej we wstępnym warunku sprawdzają, czy
     * wierzchołek przechowuje jakąś wartość.
     */
    static void inorder(std::weak_ptr<Node> node, std::function<void(T&)> &fun) {
        if (!node.lock()->hasValue())
            return;
        
        inorder(node.lock()->left, fun);
        fun(*(node.lock()->getValue().lock()));
        inorder(node.lock()->right, fun);
    }
    
    static void postorder(std::weak_ptr<Node> node, std::function<void(T&)> &fun) {
        if (!node.lock()->hasValue())
            return;

        postorder(node.lock()->left, fun);
        postorder(node.lock()->right, fun);
        fun(*(node.lock()->getValue().lock()));
    }
    
    static void preorder(std::weak_ptr<Node> node, std::function<void(T&)> &fun) {
        if (!node.lock()->hasValue())
            return;
        
        fun(*(node.lock()->getValue().lock()));
        preorder(node.lock()->left, fun);
        preorder(node.lock()->right, fun);
    }

    static Node createEmptyNode() {
        return Node();
    }

    static Node createValueNode(T val) {
        return Node(val, createEmptyNode(), createEmptyNode());
    }

    static Node createValueNode(T val, Node left, Node right) {
        return Node(val, left, right);
    }
    
    template <typename R, typename F>
    R fold(F f, R init) const {
        std::function<R(T, R, R)> fun = f;
        std::function<T(std::weak_ptr<Node>)> eval =
            [](std::weak_ptr<Node> node) {
            return *(node.lock()->getValue().lock());
        };
        
        return fold_<R, T>(fun, init, std::weak_ptr<Node>(root), eval);
    }
    
    std::size_t height() const {
        std::function<std::size_t(T, size_t, size_t)> func =
            [](T val, size_t left, size_t right) {
            return 1 + std::max(left, right);
        };
        
        return fold(func, (std::size_t) 0);
    }
    
    std::size_t size() const {
        std::function<std::size_t(T, size_t, size_t)> func =
            [](T val, size_t left, size_t right) {
            return (std::size_t) (1 + left + right);
        };
        
        return fold(func, (std::size_t) 0);
    }
    
    bool is_bst() const {
        /* O(n) dodatkowej pamięci */
        std::vector<T> temp;
        std::function<void(T&)> push_to_vector = [&temp](T &val) {
            temp.push_back(val);
        };
        apply(push_to_vector, inorder);
        
        return std::is_sorted(temp.begin(), temp.end());
    }

    template <typename F, typename U>
    U accumulate(F bin_, U init, Traversal_t traversal) const {
        std::function<U(U, T)> bin = bin_;
        /* gdyby kompilator wpadł na pomysł zrobienia U referencją
         do typu */
        U local(init);

        std::function<void(T&)> f = [&local, &bin](T &val) {
            local = bin(local, val);
        };
        
        traversal(root, f);
        return local;
    }

    template <typename F = std::function<void(T&)>>
    void apply(F op, Traversal_t traversal) const {
        std::function<void(T&)> op_ = op;
        traversal(root, op_);
    }
    
    void print(Traversal_t traversal = inorder) const {
        std::function<void(T&)> fun = [](T &val) {
            std::cout << val << " ";
        };
        
        apply(fun, traversal);
        /* Usuwa ostatnią " " z końca */
        std::cout << '\b' << std::endl;
    }

    template <typename F>
    Tree filter(F pred) const {
        // wskaźnik na korzeń podrzewa oraz najdalszy z prawej niepusty wierzchołek
        typedef std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>> RES;
        
        std::function<RES(T, RES, RES)> filter_ =
                      [&pred](T val, RES left, RES right) {
            if (pred(val)) {
                std::shared_ptr<Node> node =
                    std::make_shared<Node>(Node(val, left.first, right.first));
                
                if (right.second == nullptr) {
                    return std::make_pair(node, node);
                } else {
                    return std::make_pair(node, right.second);
                }
            } else {
                if (left.second == nullptr) {
                    return right;
                } else {
                    left.second->right = right.first;
                    return std::make_pair(left.first, right.second);
                }
            }
        };

        RES result =
            fold(filter_,
                 std::make_pair(std::make_shared<Node>(createEmptyNode()),
                                (std::shared_ptr<Node>)nullptr)); 
        
        return Tree(*(result.first));        
    }

    template <typename F, typename U = typename std::result_of<F(T)>::type>
    Tree<U> map(F transformer) const {
        std::function<typename Tree<U>::Node(T, typename Tree<U>::Node,
                                             typename Tree<U>::Node)> copy =
            [&transformer](T val, typename Tree<U>::Node left,
                           typename Tree<U>::Node right) {
            return Tree<U>::createValueNode(transformer(val), left, right);
        };
        
        return Tree<U>(fold(copy, Tree<U>::createEmptyNode()));
    }

    template <typename F, typename U = typename std::result_of<F(T)>::type>
    Tree<U> lazy_map(F transformer) const {
        typedef typename Tree<U>::Node NodeU;
        
        std::function<NodeU(std::weak_ptr<Node>,
                            NodeU, NodeU)> copy =
            [&transformer](std::weak_ptr<Node> val,
                           NodeU left,
                           NodeU right) {
            //na wszelki wypadek bierzemy shared_ptr do node, żeby nie znikł
            std::shared_ptr<Node> node_copy(val);

            // w lambdzie wszystko kopią, bo nie wiadomo, kiedy
            // to się policzy/będzie potrzebne
            NodeU node(left, right, [node_copy, transformer]() {
                    return transformer(*(node_copy->getValue().lock()));
                });
            //tutaj jest leniwość: składamy funkcję przechowywaną w node
            return node;
        };

        std::function<std::weak_ptr<Node>(std::weak_ptr<Node>)> eval = [](std::weak_ptr<Node> ptr) {
            return ptr;
        };

        NodeU res =
            fold_(copy,
                  Tree<U>::createEmptyNode(),
                  std::weak_ptr<Node>(root), eval);
        return Tree<U>(res);
    }
};

#endif // TREE_H
