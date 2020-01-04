#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "map.h"

enum COLOR {
    RED,
    BLACK
};


typedef struct Node *TreeNode;

typedef struct Node {
    Piece value;
    Point key;
    enum COLOR color;
    TreeNode left, right, parent;
} Node;

typedef struct def_map {
    TreeNode root;

    int (*compare)(Point, Point);

    void (*destructor)(Piece);

    TreeNode nil;
} def_map;

struct def_map *newMap(int (*compare)(Point, Point),
                       void (*destructor)(Piece)) {

    struct def_map *result = (def_map *) malloc(sizeof(def_map));
    result->compare = compare;
    result->destructor = destructor;

    TreeNode nil = (TreeNode) malloc(sizeof(Node));
    nil->color = BLACK;
    nil->left = nil;
    nil->right = nil;

    result->nil = nil;
    result->root = nil;

    return result;
}

void left_rotate(struct def_map *map, TreeNode node) {
    TreeNode y = node->right;
    node->right = y->left;

    if (y->left != map->nil) {
        y->left->parent = node;
    }


    y->parent = node->parent;

    if (node->parent == map->nil) {
        map->root = y;
    }
    else {
        if (node == node->parent->left) {
            node->parent->left = y;
        }
        else {
            node->parent->right = y;
        }
    }
    y->left = node;
    node->parent = y;
}


void right_rotate(struct def_map *map, TreeNode node) {
    TreeNode y = node->left;
    node->left = y->right;

    if (map->nil != y->right) {
        y->right->parent = node;
    }

    y->parent = node->parent;

    if (node->parent == map->nil) {
        map->root = y;
    }
    else {
        if (node == node->parent->left) {
            node->parent->left = y;
        }
        else {
            node->parent->right = y;
        }
    }

    y->right = node;
    node->parent = y;
}

void Tree_delete_fixup(TreeNode x, struct def_map* map) {
    TreeNode w = NULL;
    while (x != map->root && x->color == BLACK) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(map, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(map, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                left_rotate(map, x->parent);
                x = map->root;
            }
        }
        else {
            w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(map, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(map, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                right_rotate(map, x->parent);
                x = map->root;
            }
        }
    }
    x->color = BLACK;
}


TreeNode getNodeByKey(struct def_map *map, Point key) {
    TreeNode v = map->root;

    while (v != map->nil) {

        if (map->compare(key, v->key) == 1) {
            v = v->left;
            continue;
        }
        if (map->compare(key, v->key) == -1) {
            v = v->right;
            continue;
        }
        if (map->compare(key, v->key) == 0) {
            return v;
        }
    }

    return map->nil;
}

Piece getValue(struct def_map *map, Point key) {
    TreeNode node = getNodeByKey(map, key);

    if (node != map->nil) {
        return node->value;
    }
    else {
        return NULL;
    }
}

void transplant(struct def_map *map, TreeNode u, TreeNode v) {
    if (u->parent == map->nil) {
        map->root = v;
    }
    else {
        if (u == u->parent->left) {
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }
    }
    v->parent = u->parent;
}

TreeNode tree_minimum(TreeNode node, struct def_map *map) {
    TreeNode v = node;

    while (v->left != map->nil) {
        v = v->left;
    }

    return v;
}

// W przypadku zmienia implementacji uważać
// na free(z->key) na samym dole funkcji
void removeValue(struct def_map *map, Point key) {
    TreeNode z = getNodeByKey(map, key);
    TreeNode x = NULL;


    if (z != map->nil) {
        TreeNode y = z;
        enum COLOR y_original_color = y->color;
        if (z->left == map->nil) {
            x = z->right;
            transplant(map, z, z->right);
        }
        else {
            if (z->right == map->nil) {
                x = z->left;
                transplant(map, z, z->left);
            }
            else {
                y = tree_minimum(z->right, map);
                y_original_color = y->color;
                x = y->right;
                if (y->parent == z) {
                    x->parent = y;
                }
                else {
                    transplant(map, y, y->right);
                    y->right = z->right;
                    y->right->parent = y;
                }
                transplant(map, z, y);
                y->left = z->left;
                y->left->parent = y;
                y->color = z->color;
            }
        }
        if (y_original_color == BLACK) {
            Tree_delete_fixup(x, map);
        }
        free(z->key);
        free(z);
    }
}


void insertValue(struct def_map *map, Point key, Piece value) {
    TreeNode z = getNodeByKey(map, key);
    if (z == map->nil) {
        TreeNode newNode = (TreeNode) malloc(sizeof(Node));
        newNode->value = value;
        newNode->key = key;

        TreeNode x = map->root;
        TreeNode y = map->nil;

        while (x != map->nil) {
            y = x;
            if (map->compare(newNode->key, x->key) == 1) {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }

        newNode->parent = y;
        if (y == map->nil) {
            map->root = newNode;
        }
        else {
            if (map->compare(newNode->key, y->key) == 1) {
                y->left = newNode;
            }
            else {
                y->right = newNode;
            }
        }

        newNode->left = map->nil;
        newNode->right = map->nil;
        newNode->color = RED;

        while (newNode->parent->color == RED) {
            if (newNode->parent == newNode->parent->parent->left) {
                y = newNode->parent->parent->right;
                if (y->color == RED) {
                    newNode->parent->color = BLACK;
                    y->color = BLACK;
                    newNode->parent->parent->color = RED;
                    newNode = newNode->parent->parent;
                }
                else {
                    if (newNode == newNode->parent->right) {
                        newNode = newNode->parent;
                        left_rotate(map, newNode);
                    }
                    newNode->parent->color = BLACK;
                    newNode->parent->parent->color = RED;
                    right_rotate(map, newNode->parent->parent);
                }
            }
            else {
                y = newNode->parent->parent->left;
                if (y->color == RED) {
                    newNode->parent->color = BLACK;
                    y->color = BLACK;
                    newNode->parent->parent->color = RED;
                    newNode = newNode->parent->parent;
                }
                else {
                    if (newNode == newNode->parent->left) {
                        newNode = newNode->parent;
                        right_rotate(map, newNode);
                    }
                    newNode->parent->color = BLACK;
                    newNode->parent->parent->color = RED;
                    left_rotate(map, newNode->parent->parent);
                }
            }
        }
        map->root->color = BLACK;
    }
    else {
        free(key);
        map->destructor(value);
    }
}

void destroyTreeNode(Map map, TreeNode node) {
    if (node->left != map->nil) {
        destroyTreeNode(map, node->left);
    }
    if (node->right != map->nil) {
        destroyTreeNode(map, node->right);
    }

    free(node->key);
    map->destructor(node->value);
    free(node);
}

void destroyMap(struct def_map *map) {
    if (map->root != map->nil) {
        destroyTreeNode(map, map->root);
    }
    free(map->nil);
    free(map);
}
