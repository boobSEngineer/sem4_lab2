#include <iostream>
#include "list.h"


#ifndef LAB1_TREE_H
#define LAB1_TREE_H

template <typename KEY_T, typename VALUE_T>
class RbTree {
public:
    enum NodeColor : int {
        BLACK = 0,
        RED = 1
    };

    typedef typename List<KEY_T>::Iterator KeyIterator;
    typedef typename List<VALUE_T>::Iterator ValueIterator;

    // red-black tree node
    class Node {
    public:
        KEY_T key;
        VALUE_T value;
        KeyIterator key_iterator;
        ValueIterator value_iterator;
        NodeColor color = BLACK;

        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;

        Node(KeyIterator key_iter, ValueIterator value_iter) {
            this->key = *key_iter;
            this->value = *value_iter;
            key_iterator = key_iter;
            value_iterator = value_iter;
        }

        void print() {
            if (left != nullptr) {
                left->print();
            }
            std::cout << key << ": " << value << ", ";
            if (right) {
                right->print();
            }
        }

        // output tree with this node as root
        void debug_tree(int depth = 0) {
            // output left child
            if (left != nullptr) {
                left->debug_tree(depth + 1);
            }
            // output indentation
            for (int i = 0; i < depth; i++) {
                std::cout << "    ";
            }
            // output info
            std::cout << key << ":" << value << ":" << (color == RED ? "red" : "black") << "\n";
            // output right child
            if (right != nullptr) {
                right->debug_tree(depth + 1);
            }
        }

        int get_size() {
            return 1 + (left != nullptr ? left->get_size() : 0) + (right != nullptr ? right->get_size() : 0);
        }

        ~Node() {
            delete(right);
            delete(left);
        }
    };

    Node* root = nullptr;

    int get_size() {
        return root != nullptr ? root->get_size() : 0;
    }

    ~RbTree() {
        delete(root);
    }

    void clear() {
        delete(root);
        root = nullptr;
    }

    // output debug tree view
    void debug_tree() {
        if (root != nullptr) {
            root->debug_tree();
        } else {
            std::cout << "empty tree\n";
        }
    }

    // find node by key
    Node* get_node(KEY_T key) {
        Node* node = root;
        while (node != nullptr) {
            if (node->key == key) {
                return node;
            }
            if (node->key < key) {
                node = node->right;
            } else {
                node = node->left;
            }
        }
        return nullptr;
    }

    // left-rotate given node
    void left_rotate(Node* node) {
        Node* tmp = node->right;
        node->right = tmp->left;
        if (tmp->left != nullptr) {
            tmp->left->parent = node;
        }
        tmp->parent = node->parent;

        if (node->parent == nullptr) {
            root = tmp;
        } else {
            if (node == node->parent->left) {
                node->parent->left = tmp;
            } else {
                node->parent->right = tmp;
            }
        }
        tmp->left = node;
        node->parent = tmp;
    }

    // right-rotate given node
    void right_rotate(Node* node) {
        Node* tmp = node->left;
        node->left = tmp->right;
        if (tmp->right != nullptr) {
            tmp->right->parent = node;
        }
        tmp->parent = node->parent;

        if (node->parent == nullptr) {
            root = tmp;
        } else {
            if (node == node->parent->left) {
                node->parent->left = tmp;
            } else {
                node->parent->right = tmp;
            }
        }
        tmp->right = node;
        node->parent = tmp;
    }

    // re-balance tree after insertion
    void insert_fixup(Node* node) {
        while (node->parent != nullptr && node->parent->color == RED) {
            if (node->parent == node->parent->parent->left) {
                Node* tmp = node->parent->parent->right;
                if (tmp != nullptr && tmp->color == RED) {
                    node->parent->color = BLACK;
                    tmp->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->right) {
                        node = node->parent;
                        left_rotate(node);
                    }
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    right_rotate(node->parent->parent);
                }
            } else {
                Node* tmp = node->parent->parent->left;
                if (tmp != nullptr && tmp->color == RED) {
                    node->parent->color = BLACK;
                    tmp->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        right_rotate(node);
                    }
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    left_rotate(node->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    // inserts node into tree and re-balances tree
    // if node with same key exists, replaces its value and returns false, in this case it must be deleted
    bool insert(Node* node) {
        Node* last_node = nullptr;
        Node* current_node = root;
        while (current_node != nullptr) {
            last_node = current_node;
            if (node->key == current_node->key) {
                current_node->value = node->value;
                return false;
            }
            if (node->key < current_node->key) {
                current_node = current_node->left;
            } else {
                current_node = current_node->right;
            }
        }
        node->parent = last_node;
        if (last_node == nullptr) {
            root = node;
        } else if (node->key < last_node->key) {
            last_node->left = node;
        } else {
            last_node->right = node;
        }
        node->left = node->right = nullptr;
        node->color = RED;
        insert_fixup(node);
        return true;
    }

    // re-balance tree after removal
    void remove_fixup(Node* node) {
        while (node != root && (node == nullptr || node->color == BLACK)) {
            if (node == node->parent->left) {
                Node* w = node->parent->right;
                if (w != nullptr && w->color == RED) {
                    w->color = BLACK;
                    node->parent->color = RED;
                    left_rotate(node->parent);
                    w = node->parent->right;
                }
                if (w == nullptr) {
                    break;
                }
                if ((w->left == nullptr || w->left->color == BLACK) &&
                    (w->right == nullptr || w->right->color == BLACK)) {
                    w->color = RED;
                    node = node->parent;
                } else {
                    if (w->right == nullptr || w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        right_rotate(w);
                        w = node->parent->right;
                    }
                    w->color = node->parent->color;
                    node->parent->color = BLACK;
                    w->right->color = BLACK;
                    left_rotate(node->parent);
                    node = root;
                }
            } else {
                Node* w = node->parent->left;
                if (w != nullptr && w->color == RED) {
                    w->color = BLACK;
                    node->parent->color = RED;
                    right_rotate(node->parent);
                    w = node->parent->left;
                }
                if (w == nullptr) {
                    break;
                }
                if ((w->left == nullptr || w->left->color == BLACK) &&
                    (w->right == nullptr || w->right->color == BLACK)) {
                    w->color = RED;
                    node = node->parent;
                } else {
                    if (w->left == nullptr || w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        left_rotate(w);
                        w = node->parent->left;
                    }
                    w->color = node->parent->color;
                    node->parent->color = BLACK;
                    w->left->color = BLACK;
                    right_rotate(node->parent);
                    node = root;
                }

            }
        }
    }

    Node* minimal_node(Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Node* tree_successor(Node* node) {
        if (node->right != nullptr) {
            return minimal_node(node);
        }
        Node* tmp = node->parent;
        while (tmp != nullptr && node == tmp->right) {
            node = tmp;
            tmp = tmp->parent;
        }
        return tmp;
    }

    Node* remove(Node* node) {
        Node* y;
        if (node->left == nullptr || node->right == nullptr) {
            y = node;
        } else {
            y = tree_successor(node);
        }
        if (y == nullptr) {
            debug_tree();
            std::cout << " " << node->key << " ";
        }
        Node* x;
        if (y->left != nullptr) {
            x = y->left;
        } else {
            x = y->right;
        }

        if (x != nullptr) {
            x->parent = y->parent;
        }
        if (y->parent == nullptr) {
            root = x;
        } else {
            if (y == y->parent->left) {
                y->parent->left = x;
            } else {
                y->parent->right = x;
            }
        }
        if (y != node) {
            node->key = y->key;
            node->value = y->value;
        }
        if (y->color == BLACK && x != nullptr) {
            remove_fixup(x);
        }
        return y;
    }


};

#endif