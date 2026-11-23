#pragma once

template <typename T>
struct Array {
    T* data = nullptr;
    int size = 0;
    int capacity = 0;

    void Init() {
        capacity = 1;
        size = 0;
        data = new T[capacity];
    }

    void MPush(const T& value) {
        if (size == capacity) {
            T* next = new T[capacity * 2];
            for (int i = 0; i < size; ++i)
                next[i] = data[i];
            delete[] data;
            data = next;
            capacity *= 2;
        }
        data[size++] = value;
    }

    void MDelInd(int index) {
        if (index < 0 || index >= size)
            return;
        for (int i = index; i < size - 1; ++i)
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
