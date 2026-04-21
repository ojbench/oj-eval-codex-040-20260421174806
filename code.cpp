// Problem 040: Doubly linked list with shared_ptr/weak_ptr
#include <iostream>
#include <memory>
#include <optional>
#include <string>

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T value;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;
        explicit Node(const T& val) : value(val), next(nullptr) {}
    };

    std::shared_ptr<Node> head;
    std::weak_ptr<Node> tail;
    size_t length{0};

public:
    class iterator {
        std::shared_ptr<Node> current;
    public:
        explicit iterator(std::shared_ptr<Node> n) : current(std::move(n)) {}
        T& operator*() { return current->value; }
        iterator operator++(int) { iterator tmp=*this; if(current) current=current->next; return tmp; }
        bool operator==(const iterator& other) const { return current==other.current; }
        bool operator!=(const iterator& other) const { return !(*this==other); }
    };

    DoublyLinkedList() = default;

    bool empty() const { return length==0; }
    size_t size() const { return length; }

    void push_front(const T& value) {
        auto n = std::make_shared<Node>(value);
        n->next = head;
        if (head) {
            head->prev = n;
        } else {
            tail = n;
        }
        head = std::move(n);
        ++length;
    }

    void push_back(const T& value) {
        auto n = std::make_shared<Node>(value);
        auto t = tail.lock();
        if (!t) {
            head = n;
            tail = n;
        } else {
            t->next = n;
            n->prev = t;
            tail = n;
        }
        ++length;
    }

    std::optional<T> pop_front() {
        if (!head) return std::nullopt;
        T ret = std::move(head->value);
        head = head->next;
        if (head) {
            head->prev.reset();
        } else {
            tail.reset();
        }
        --length;
        return ret;
    }

    std::optional<T> pop_back() {
        if (empty()) return std::nullopt;
        auto t = tail.lock();
        T ret = std::move(t->value);
        auto p = t->prev.lock();
        if (p) {
            p->next.reset();
            tail = p;
        } else {
            head.reset();
            tail.reset();
        }
        --length;
        return ret;
    }

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }
};

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    DoublyLinkedList<long long> lst;
    int q; if(!(std::cin>>q)) return 0;
    while (q--) {
        std::string op;
        std::cin >> op;
        if (op == push_front) {
            long long x; std::cin >> x; lst.push_front(x);
        } else if (op == push_back) {
            long long x; std::cin >> x; lst.push_back(x);
        } else if (op == pop_front) {
            auto v = lst.pop_front();
            if (v) std::cout << *v << n;
            else std::cout << EMPTYn;
        } else if (op == pop_back) {
            auto v = lst.pop_back();
            if (v) std::cout << *v << n;
            else std::cout << EMPTYn;
        } else if (op == size) {
            std::cout << lst.size() << n;
        } else if (op == empty) {
            std::cout << (lst.empty() ? YES : NO) << n;
        } else if (op == iter) {
            bool first = true;
            for (auto it = lst.begin(); it != lst.end(); it++) {
                if (!first) std::cout << ' ';
                first = false;
                std::cout << *it;
            }
            std::cout << n;
        } else {
            std::string line; std::getline(std::cin, line);
        }
    }
    return 0;
}
