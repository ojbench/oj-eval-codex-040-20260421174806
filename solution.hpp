#include <iostream>
#include <memory>
#include <optional>

// Implementation of a doubly linked list using std::shared_ptr/std::weak_ptr
// Mirrors the problem skeleton API and behavior.

template <typename T>
class DoublyLinkedList {
private:
    // Node structure definition
    struct Node {
        T value;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;

        explicit Node(const T& val) : value(val), next(nullptr) {}
    };

    std::shared_ptr<Node> head;
    std::weak_ptr<Node> tail;
    size_t length;

public:
    // Iterator class
    class iterator {
    private:
        std::shared_ptr<Node> current;

    public:
        explicit iterator(std::shared_ptr<Node> node) : current(std::move(node)) {}

        // Dereference operator to get value
        T& operator*() {
            return current->value;
        }

        // Post-increment operator
        iterator operator++(int) {
            iterator tmp = *this;
            if (current) {
                current = current->next;
            }
            return tmp;
        }

        // Equality operators
        bool operator==(const iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    // Constructor
    DoublyLinkedList() : head(nullptr), length(0) {}

    // No destructor needed, memory managed by shared_ptr

    // Check if list is empty
    bool empty() const {
        return length == 0;
    }

    // Get the size of the list
    size_t size() const {
        return length;
    }

    // Add element to the front
    void push_front(const T& value) {
        auto new_node = std::make_shared<Node>(value);
        new_node->next = head;
        if (head) {
            head->prev = new_node; // weak_ptr from new_node
        } else {
            // list was empty, new node is also tail
            tail = new_node;
        }
        head = std::move(new_node);
        ++length;
    }

    // Add element to the back
    void push_back(const T& value) {
        auto new_node = std::make_shared<Node>(value);
        auto t = tail.lock();
        if (!t) {
            // empty list
            head = new_node;
            tail = new_node;
        } else {
            t->next = new_node;
            new_node->prev = t; // weak_ptr from t
            tail = new_node;
        }
        ++length;
    }

    // Remove and return the first element
    std::optional<T> pop_front() {
        if (!head) {
            return std::nullopt;
        }
        T ret = std::move(head->value);
        head = head->next;
        if (head) {
            head->prev.reset();
        } else {
            // list becomes empty
            tail.reset();
        }
        --length;
        return ret;
    }

    // Remove and return the last element
    std::optional<T> pop_back() {
        if (empty()) {
            return std::nullopt;
        }
        auto t = tail.lock();
        T ret = std::move(t->value);
        auto prev = t->prev.lock();
        if (prev) {
            prev->next.reset();
            tail = prev;
        } else {
            // removing the last element
            head.reset();
            tail.reset();
        }
        --length;
        return ret;
    }

    // Iterators
    iterator begin() {
        return iterator(head);
    }

    iterator end() {
        return iterator(nullptr);
    }
};
