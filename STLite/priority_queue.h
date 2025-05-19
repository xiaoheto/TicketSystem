#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.h"

namespace sjtu {
    template<typename T, class Compare = std::less<T>>
    class priority_queue {
    private:
        struct Node {
            Node *left;
            Node *right;
            T data;
            Node(const T &value, Node *l = nullptr, Node *r = nullptr) : data(value), left(l), right(r) {}
        };
        Node *root_;
        size_t size_;

        Node *Copy(Node *other) {
            if (!other) {
                return nullptr;
            }
            Node *newNode = new Node(other->data);
            newNode->left = Copy(other->left);
            newNode->right = Copy(other->right);
            return newNode;
        }

        void Clear(Node *node) {
            if (!node) {
                return;
            }
            Clear(node->left);
            Clear(node->right);
            delete node;
        }

        Node *Merge(Node *node1, Node *node2) {
            if (!node1) {
                return node2;
            }
            if (!node2) {
                return node1;
            }
            try {
                if (Compare()(node1->data, node2->data)) {
                    Node *temp = node2->left;
                    node2->left = Merge(node1, node2->right);
                    node2->right = temp;
                    return node2;
                } else {
                    Node *temp = node1->left;
                    node1->left = Merge(node2, node1->right);
                    node1->right = temp;
                    return node1;
                }
            } catch (...) {
                throw;
            }
        }

    public:
        priority_queue() : root_(nullptr), size_(0) {}

        priority_queue(const priority_queue &other) : root_(Copy(other.root_)), size_(other.size_) {}

        ~priority_queue() {
            Clear(root_);
        }

        priority_queue &operator=(const priority_queue &other) {
            if (this == &other) {
                return *this;
            }
            Clear(root_);
            root_ = Copy(other.root_);
            size_ = other.size_;
            return *this;
        }

        const T &top() const {
            if (empty()) {
                throw container_is_empty();
            }
            return root_->data;
        }

        void push(const T &e) {
            Node *node = new Node(e);
            try {
                root_ = Merge(root_, node);
                size_++;
            } catch (...) {
                delete node;
                throw;
            }
        }

        void pop() {
            if (empty()) {
                throw container_is_empty();
            }
            Node *temp = root_;
            try {
                root_ = Merge(root_->left, root_->right);
                size_--;
                delete temp;
            } catch (...) {
                root_ = temp;
                throw;
            }
        }

        size_t size() const {
            return size_;
        }

        bool empty() const {
            return size_ == 0;
        }

        void merge(priority_queue &other) {
            try {
                root_ = Merge(root_, other.root_);
                size_ += other.size_;
                other.root_ = nullptr;
                other.size_ = 0;
            } catch (...) {
                throw;
            }
        }
    };
} // namespace sjtu

#endif