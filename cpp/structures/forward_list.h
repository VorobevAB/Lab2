#pragma once

template <typename T>
struct ForwardList {
    struct Node {
        T value;
        Node* next;
    };

    Node* head = nullptr;

    void FPushFront(const T& value) {
        head = new Node{value, head};
    }

    void FPushBack(const T& value) {
        Node* node = new Node{value, nullptr};
        if (!head) {
            head = node;
            return;
        }
        Node* current = head;
        while (current->next)
            current = current->next;
        current->next = node;
    }

    void FDelValue(const T& value) {
        Node* current = head;
        Node* previous = nullptr;
        while (current && !(current->value == value)) {
            previous = current;
            current = current->next;
        }
        if (!current)
            return;
        if (previous)
            previous->next = current->next;
        else
            head = current->next;
        delete current;
    }

    void clear() {
        while (head) {
            Node* next = head->next;
            delete head;
            head = next;
        }
    }
};
