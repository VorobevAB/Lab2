#pragma once

template <typename T>
struct Stack {
    struct Node {
        T value;
        Node* next;
    };

    Node* head = nullptr;

    void Init() {
        clear();
    }

    bool SEmpty() const {
        return head == nullptr;
    }

    void SPush(const T& value) {
        head = new Node{value, head};
    }

    T STop() const {
        if (SEmpty())
            return T{};
        return head->value;
    }

    void SPop() {
        if (SEmpty())
            return;
        Node* node = head;
        head = head->next;
        delete node;
    }

    void clear() {
        while (head) {
            Node* next = head->next;
            delete head;
            head = next;
        }
    }
};
