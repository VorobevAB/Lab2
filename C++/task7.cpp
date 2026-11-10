#include "task7.h"
#include "structures.h"
#include <iostream>
#include <string>

using namespace std;

struct CacheItem {
    int key;
    int value;
    int freq;
    int last;
};

struct LFUCache {
    Array<CacheItem> data;
    int capacity;
    int timer = 0;

    LFUCache(int cap) {
        capacity = cap;
        data.Init();
    }

    int find(int key) const {
        for (int i = 0; i < data.size; ++i)
            if (data.data[i].key == key)
                return i;
        return -1;
    }

    int get(int key) {
        int idx = find(key);
        if (idx == -1)
            return -1;

        ++data.data[idx].freq;
        data.data[idx].last = ++timer;
        return data.data[idx].value;
    }

    void set(int key, int value) {
        if (capacity <= 0)
            return;

        int idx = find(key);
        if (idx != -1) {
            data.data[idx].value = value;
            ++data.data[idx].freq;
            data.data[idx].last = ++timer;
            return;
        }

        if (data.size == capacity) {
            int del = 0;
            for (int i = 1; i < data.size; ++i) {
                if (data.data[i].freq < data.data[del].freq ||
                    (data.data[i].freq == data.data[del].freq &&
                     data.data[i].last < data.data[del].last))
                    del = i;
            }
            data.MDelInd(del);
        }

        CacheItem item{ key, value, 1, ++timer };
        data.MPush(item);
    }

    void clear() {
        data.clear();
    }
};

void task7() {
    int cap, q;
    if (!(cin >> cap >> q)) {
        cout << "ОШИБКА: ожидалось cap q" << endl;
        return;
    }
    if (cap < 0 || q < 0) {
        cout << "ОШИБКА: cap >= 0, q >= 0" << endl;
        return;
    }

    LFUCache cache(cap);
    for (int i = 0; i < q; ++i) {
        string op;
        int x;
        if (!(cin >> op >> x)) {
            cout << "ОШИБКА: ожидалось <op> <key> в запросе #" << i + 1 << endl;
            cache.clear();
            return;
        }

        if (op == "SET") {
            int y;
            if (!(cin >> y)) {
                cout << "ОШИБКА: SET требует ключ и значение" << endl;
                cache.clear();
                return;
            }
            cache.set(x, y);
        }
        else if (op == "GET") {
            cout << cache.get(x) << endl;
        }
        else {
            cout << "ОШИБКА: неизвестная операция '" << op << "'" << endl;
            cache.clear();
            return;
        }
    }

    cache.clear();
}
