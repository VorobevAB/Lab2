#include "task7.h"
#include "structures/array.h"
#include <iostream>
#include <string>
using namespace std;

struct CacheItem {
    int key;
    int value;
    int frequency;
    int lastAccess;
};

struct LFUCache {
    Array<CacheItem> data;
    int capacity = 0;
    int timer = 0;

    void Init(int cap) {
        capacity = cap;
        timer = 0;
        data.Init();
    }

    int find(int key) const {
        for (int i = 0; i < data.size; ++i)
            if (data.data[i].key == key)
                return i;
        return -1;
    }

    int get(int key) {
        int index = find(key);
        if (index == -1)
            return -1;
        ++data.data[index].frequency;
        data.data[index].lastAccess = ++timer;
        return data.data[index].value;
    }

    void set(int key, int value) {
        int index = find(key);
        if (index != -1) {
            data.data[index].value = value;
            ++data.data[index].frequency;
            data.data[index].lastAccess = ++timer;
            return;
        }
        if (capacity <= 0)
            return;

        if (data.size == capacity) {
            int removeIndex = 0;
            for (int i = 1; i < data.size; ++i)
                if (data.data[i].frequency < data.data[removeIndex].frequency ||
                    (data.data[i].frequency == data.data[removeIndex].frequency &&
                     data.data[i].lastAccess < data.data[removeIndex].lastAccess))
                    removeIndex = i;
            data.MDelInd(removeIndex);
        }
        data.MPush(CacheItem{key, value, 1, ++timer});
    }

    void clear() { data.clear(); }
};

void task7() {
    int capacity, q;
    cin >> capacity >> q;
    LFUCache cache;
    cache.Init(capacity);
    for (int i = 0; i < q; ++i) {
        string operation;
        int key;
        cin >> operation >> key;
        if (operation == "SET") {
            int value;
            cin >> value;
            cache.set(key, value);
        } else if (operation == "GET") {
            cout << cache.get(key) << endl;
        }
    }
    cache.clear();
}
