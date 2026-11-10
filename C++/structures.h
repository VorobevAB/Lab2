#pragma once
#include <iostream>

using namespace std;

template <typename T>
struct Array {
    T* data;
    int size;
    int capacity;

    void Init() {
        capacity = 1;
        size = 0;
        data = new T[capacity];
    }

    void MPush(const T& n) {
        if (size == capacity) {
            T* nd = new T[capacity * 2];
            for (int i = 0; i < size; ++i)
                nd[i] = data[i];
            delete[] data;
            data = nd;
            capacity *= 2;
        }
        data[size++] = n;
    }

    void MDelInd(int idx) {
        if (idx < 0 || idx >= size)
            return;
        for (int i = idx; i < size - 1; ++i)
            data[i] = data[i + 1];
        --size;
    }

    void clear() {
        delete[] data;
        data = nullptr;
        size = 0;
        capacity = 0;
    }
};

template <typename T>
struct Node {
    T value;
    Node<T>* next = nullptr;
};

template <typename T>
struct ForwardList {
    Node<T>* head = nullptr;

    void FPushFront(const T& v) {
        head = new Node<T>{ v, head };
    }

    void FPushBack(const T& v) {
        Node<T>* n = new Node<T>{ v, nullptr };
        if (!head) {
            head = n;
            return;
        }
        Node<T>* cur = head;
        while (cur->next)
            cur = cur->next;
        cur->next = n;
    }

    void FDelValue(const T& v) {
        Node<T>* cur = head;
        Node<T>* prev = nullptr;
        while (cur && cur->value != v) {
            prev = cur;
            cur = cur->next;
        }
        if (!cur)
            return;
        if (prev)
            prev->next = cur->next;
        else
            head = cur->next;
        delete cur;
    }

    void clear() {
        while (head) {
            Node<T>* n = head->next;
            delete head;
            head = n;
        }
    }
};

template <typename T>
struct Stack {
    Node<T>* head = nullptr;

    bool SEmpty() const {
        return head == nullptr;
    }

    void SPush(const T& v) {
        head = new Node<T>{ v, head };
    }

    T STop() const {
        if (SEmpty())
            return T{};
        return head->value;
    }

    void SPop() {
        if (SEmpty())
            return;
        Node<T>* d = head;
        head = d->next;
        delete d;
    }

    void clear() {
        while (head) {
            Node<T>* n = head->next;
            delete head;
            head = n;
        }
    }
};
