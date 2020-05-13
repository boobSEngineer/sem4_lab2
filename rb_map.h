#include "rb_tree.h"
#include "list.h"

#ifndef LAB1_RB_MAP_H
#define LAB1_RB_MAP_H

template <typename KEY_T, typename VALUE_T>
class RbMap {
public:
    class invalid_key_exception : public std::exception {

    };

private:
    typedef typename RbTree<KEY_T, VALUE_T>::Node Node;
    RbTree<KEY_T, VALUE_T> tree;
    List<KEY_T> key_list;
    List<VALUE_T> value_list;

public:
    bool insert(KEY_T key, VALUE_T value) {
        Node* found = tree.get_node(key);
        if (found != nullptr) {
            found->value = value;
            *(found->value_iterator) = value;
            return false;
        } else {
            Node* node = new Node(key_list.add(key), value_list.add(value));
            tree.insert(node);
            return true;
        }
    }

    bool remove(KEY_T key) {
        Node* node = tree.get_node(key);
        if (node != nullptr) {
            node = tree.remove(node);
            key_list.erase(node->key_iterator);
            value_list.erase(node->value_iterator);
            node->right = node->left = node->parent = nullptr;
            delete(node);
            return true;
        }
        return false;
    }

    Node* find(KEY_T key) {
        return tree.get_node(key);
    }

    bool has(KEY_T key) {
        return find(key) != nullptr;
    }

    VALUE_T& get(KEY_T key) {
        Node* node = tree.get_node(key);
        if (node != nullptr) {
            return node->value;
        }
        std::cout << "invalid map key '" << key << "'\n";
        throw invalid_key_exception();
    }

    void print() {
        std::cout << "{";
        if (tree.root != nullptr) {
            tree.root->print();
        }
        std::cout << "}\n";
    }

    void debug() {
        std::cout << "debug map tree:\n";
        tree.debug_tree();
        std::cout << "\n";
    }

    List<KEY_T>& get_keys() {
        return key_list;
    }

    List<VALUE_T>& get_values() {
        return value_list;
    }

    int get_length() {
        return tree.get_size();
    }

    void clear() {
        tree.clear();
        key_list.clear();
        value_list.clear();
    }
};

#endif //LAB1_RB_MAP_H