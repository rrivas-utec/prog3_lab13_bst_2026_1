//
// Created by rrivas on 19/06/2026.
//

#ifndef PROG3_LAB13_BST_2026_1_MAP_UTEC_H
#define PROG3_LAB13_BST_2026_1_MAP_UTEC_H

#include <functional>

namespace utec {
    template<
        class Key,
        class T,
        class Compare = std::less<Key>
    >
    class map {
        struct node_t {
            std::pair<Key, T> value;
            std::unique_ptr<node_t> left;
            std::unique_ptr<node_t> right;
            node_t* parent = nullptr;
        };

        static node_t* minimum(node_t* node) {
            while (node != nullptr && node->left != nullptr) {
                node = node->left.get();
            }
            return node;
        }
        static node_t* maximum(node_t* node) {
            while (node != nullptr && node->right != nullptr) {
                node = node->right.get();
            }
            return node;
        }

        std::unique_ptr<node_t> root;
        size_t sz = 0;
        Compare comp;

    public:
        class iterator {
        private:
            node_t* current = nullptr;
            node_t* root = nullptr;
            friend class map;

            iterator(node_t* node, node_t* root): current(node), root(root) {}
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using difference_type = std::ptrdiff_t;

            // pre - incremento
            iterator& operator++() {
                if (current_ == nullptr) return *this;

                if (current->right != nullptr) {
                    current = minimum(current->right);
                    return *this;
                }

                current = current->parent;
                return *this;
            }

            // post incremento
            iterator operator++(int) {
                auto cpy = *this;
                ++(*this);
                return cpy;
            }


            iterator& operator--() {
                if (current_ == nullptr) {
                    current = maximum(root);
                    return *this;
                }

                if (current->left != nullptr) {
                    current = maximum(current->left);
                    return *this;
                }

                current = current->parent;
                return *this;
            }
            iterator operator--(int) {
                auto cpy = *this;
                --(*this);
                return cpy;
            }
            reference operator*() const {
                return current->value;
            }
            reference operator->() const {
                return &(current->value);
            }
            bool operator==(const iterator& rhs) const {
                return current == rhs.current;
            }
            bool operator!=(const iterator& rhs) const {
                return !(*this == other);
            }

        };

    public:
        map() = default;
        map(const map&) = delete;
        map& operator=(const map&) = delete;

        map(map&&) noexcept = default;
        map& operator=(map&&) noexcept = default;

        ~map() = default;

        int size() const { return sz; }
        bool empty() const { return sz == 0; }
        bool insert(Key key, T value) {
            std::unique_ptr<node_t>* link = &root;
            node_t* parent = nullptr;
            while (link != nullptr) {
                parent = link->get();

                if (comp(key, (*link)->value.first)) {
                    link = &(*link)->left;
                }
                else if (comp((*link)->value.first, key)) {
                    link = &(*link)->right;
                }
                else {
                    return false;
                }
            }
            
            *link = std::make_unique<node_t>(key, value, parent);
            ++sz;
            return true;
        }
        bool contains(Key key) const { return false; }
        iterator find(Key key) const { return iterator(); }
        iterator begin() const { return iterator(); }
        iterator end() const { return iterator(); }

    };
}

#endif //PROG3_LAB13_BST_2026_1_MAP_UTEC_H