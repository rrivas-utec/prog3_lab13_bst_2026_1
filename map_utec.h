//
// Created by rrivas on 19/06/2026.
//

#ifndef PROG3_LAB13_BST_2026_1_MAP_UTEC_H
#define PROG3_LAB13_BST_2026_1_MAP_UTEC_H

#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <utility>

namespace utec {
    template<
        class Key,
        class T,
        class Compare = std::less<Key>
    >
    class map {
        struct node_t {
            std::pair<const Key, T> data;
            std::unique_ptr<node_t> left = nullptr;
            std::unique_ptr<node_t> right = nullptr;
            node_t* parent = nullptr;
            node_t(Key key, T value, node_t* parent): data(std::move(key), std::move(value)), parent(parent) {};
        };

        static node_t* minimum(node_t* node) {
            while (node != nullptr && node->left != nullptr) {
                node = node->left.get();
            }
            return node;
        }
        static const node_t* minimum(const node_t* node) {
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
        static const node_t* maximum(const node_t* node) {
            while (node != nullptr && node->right != nullptr) {
                node = node->right.get();
            }
            return node;
        }

        std::unique_ptr<node_t> root;
        std::size_t sz = 0;
        Compare comp;

    public:
        class const_iterator;

        class iterator {
        private:
            node_t* current = nullptr;
            node_t* root = nullptr;
            friend class map;
            friend class const_iterator;

            iterator(node_t* node, node_t* root): current(node), root(root) {}
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = std::pair<const Key, T>;
            using pointer = value_type*;
            using reference = value_type&;
            using difference_type = std::ptrdiff_t;

            // pre - incremento
            iterator& operator++() {
                if (current == nullptr) return *this;

                if (current->right != nullptr) {
                    current = minimum(current->right.get());
                    return *this;
                } else {
                    node_t* child = current;
                    node_t* parent = current->parent;

                    while (parent != nullptr && child == parent->right.get()) {
                        child = parent;
                        parent = parent->parent;
                    }
                    current = parent;
                }
                return *this;
            }
            // post incremento
            iterator operator++(int) {
                auto cpy = *this;
                ++(*this);
                return cpy;
            }

            iterator& operator--() {
                if (current == nullptr) {
                    current = maximum(root);
                    return *this;
                }

                if (current->left != nullptr) {
                    current = maximum(current->left.get());
                    return *this;
                }
                else {
                    node_t* child = current;
                    node_t* parent = current->parent;

                    while (parent != nullptr && child == parent->left.get()) {
                        child = parent;
                        parent = parent->parent;
                    }

                    current = parent;
                }
                return *this;
            }
            iterator operator--(int) {
                auto cpy = *this;
                --(*this);
                return cpy;
            }
            reference operator*() {
                return current->data;
            }
            reference operator*() const {
                return current->data;
            }
            pointer operator->() const {
                return &(current->data);
            }
            bool operator==(const iterator& rhs) const {
                return current == rhs.current;
            }
            bool operator!=(const iterator& rhs) const {
                return !(*this == rhs);
            }
        };

        class const_iterator {
        private:
            const node_t* current = nullptr;
            const node_t* root = nullptr;
            friend class map;

            const_iterator(const node_t* node, const node_t* root): current(node), root(root) {}
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = std::pair<const Key, T>;
            using pointer = const value_type*;
            using reference = const value_type&;
            using difference_type = std::ptrdiff_t;

            const_iterator(const iterator& other): current(other.current), root(other.root) {}

            const_iterator& operator++() {
                if (current == nullptr) return *this;

                if (current->right != nullptr) {
                    current = minimum(current->right.get());
                    return *this;
                } else {
                    const node_t* child = current;
                    const node_t* parent = current->parent;

                    while (parent != nullptr && child == parent->right.get()) {
                        child = parent;
                        parent = parent->parent;
                    }
                    current = parent;
                }
                return *this;
            }
            const_iterator operator++(int) {
                auto cpy = *this;
                ++(*this);
                return cpy;
            }

            const_iterator& operator--() {
                if (current == nullptr) {
                    current = maximum(root);
                    return *this;
                }

                if (current->left != nullptr) {
                    current = maximum(current->left.get());
                    return *this;
                }
                else {
                    const node_t* child = current;
                    const node_t* parent = current->parent;

                    while (parent != nullptr && child == parent->left.get()) {
                        child = parent;
                        parent = parent->parent;
                    }

                    current = parent;
                }
                return *this;
            }
            const_iterator operator--(int) {
                auto cpy = *this;
                --(*this);
                return cpy;
            }
            reference operator*() const {
                return current->data;
            }
            pointer operator->() const {
                return &(current->data);
            }
            bool operator==(const const_iterator& rhs) const {
                return current == rhs.current;
            }
            bool operator!=(const const_iterator& rhs) const {
                return !(*this == rhs);
            }
        };

    public:
        map() = default;
        map(const map&) = delete;
        map& operator=(const map&) = delete;

        map(map&&) noexcept = default;
        map& operator=(map&&) noexcept = default;

        ~map() = default;

        [[nodiscard]] std::size_t size() const { return sz; }
        [[nodiscard]] bool empty() const { return sz == 0; }
        bool insert(Key key, T value) {
            std::unique_ptr<node_t>* link = &root;
            node_t* parent = nullptr;
            while (*link != nullptr) {
                parent = link->get();
                if (comp(key, (*link)->data.first)) {
                    link = &(*link)->left;
                }
                else if (comp((*link)->data.first, key)) {
                    link = &(*link)->right;
                }
                else {
                    return false;
                }
            }
            
            *link = std::make_unique<node_t>(std::move(key), std::move(value), parent);
            ++sz;
            return true;
        }
        iterator find(const Key& key) {
            node_t* current = root.get();
            while (current != nullptr) {
                if (comp(key, current->data.first)) {
                    current = current->left.get();
                }
                else if (comp(current->data.first, key)) {
                    current = current->right.get();
                }
                else {
                    return iterator(current, root.get());
                }
            }
            return end();
        }
        const_iterator find(const Key& key) const {
            const node_t* root_node = root.get();
            const node_t* current = root_node;
            while (current != nullptr) {
                if (comp(key, current->data.first)) {
                    current = current->left.get();
                }
                else if (comp(current->data.first, key)) {
                    current = current->right.get();
                }
                else {
                    return const_iterator(current, root_node);
                }
            }
            return end();
        }
        iterator begin() { return iterator(minimum(root.get()), root.get()); }
        iterator end() { return iterator(nullptr, root.get()); }
        const_iterator begin() const {
            const node_t* root_node = root.get();
            return const_iterator(minimum(root_node), root_node);
        }
        const_iterator end() const {
            const node_t* root_node = root.get();
            return const_iterator(nullptr, root_node);
        }
        const_iterator cbegin() const { return begin(); }
        const_iterator cend() const { return end(); }
        void clear() {
            root.reset();
            sz = 0;
        }
    };
}

#endif //PROG3_LAB13_BST_2026_1_MAP_UTEC_H
