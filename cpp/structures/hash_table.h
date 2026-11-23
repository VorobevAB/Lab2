#pragma once
#include "forward_list.h"
#include "array.h"
#include <string>
#include <utility>

using namespace std;

inline size_t hashString(const string& value) {
    size_t hash = 0;
    for (unsigned char c : value)
        hash = hash * 31 + c;
    return hash;
}

template <typename V>
struct HashEntry {
    string key;
    V value;
};

template <typename V>
struct ChainHashMap {
    static const int CAP = 101;
    ForwardList<HashEntry<V>> table[CAP];
    unsigned long long attempts = 0;

    int h(const string& key) const {
        return static_cast<int>(hashString(key) % CAP);
    }

    void set(const string& key, const V& value) {
        int index = h(key);
        for (typename ForwardList<HashEntry<V>>::Node* node = table[index].head;
             node; node = node->next) {
            ++attempts;
            if (node->value.key == key) {
                node->value.value = value;
                return;
            }
        }
        table[index].FPushFront(HashEntry<V>{key, value});
    }

    bool get(const string& key, V& value) {
        int index = h(key);
        for (typename ForwardList<HashEntry<V>>::Node* node = table[index].head;
             node; node = node->next) {
            ++attempts;
            if (node->value.key == key) {
                value = node->value.value;
                return true;
            }
        }
        return false;
    }

    bool contains(const string& key) {
        V value{};
        return get(key, value);
    }

    void del(const string& key) {
        int index = h(key);
        typename ForwardList<HashEntry<V>>::Node* current = table[index].head;
        typename ForwardList<HashEntry<V>>::Node* previous = nullptr;
        while (current && current->value.key != key) {
            previous = current;
            current = current->next;
        }
        if (!current)
            return;
        if (previous)
            previous->next = current->next;
        else
            table[index].head = current->next;
        delete current;
    }

    void clear() {
        for (int i = 0; i < CAP; ++i)
            table[i].clear();
    }
};

struct StringSet {
    ChainHashMap<int> items;

    void add(const string& key) { items.set(key, 1); }
    void del(const string& key) { items.del(key); }
    bool contains(const string& key) { return items.contains(key); }
    void clear() { items.clear(); }
};

struct CuckooHashMap {
    struct Entry {
        string key;
        string value;
        bool used;
    };

    Array<Entry> first;
    Array<Entry> second;
    size_t seed1 = 0;
    size_t seed2 = 1;
    unsigned long long attempts = 0;
    unsigned long long displacements = 0;
    unsigned long long rebuildAttempts = 0;
    unsigned long long lookupChecks = 0;

    void Init(int capacity = 16) {
        first.Init();
        second.Init();
        for (int i = 0; i < capacity; ++i) {
            first.MPush(Entry{"", "", false});
            second.MPush(Entry{"", "", false});
        }
    }

    int h1(const string& key) const {
        return static_cast<int>((hashString(key) + seed1) % first.size);
    }

    int h2(const string& key) const {
        return static_cast<int>((hashString(key) * 33 + seed2) % second.size);
    }

    bool get(const string& key, string& value) {
        ++lookupChecks;
        if (first.data[h1(key)].used && first.data[h1(key)].key == key) {
            value = first.data[h1(key)].value;
            return true;
        }
        ++lookupChecks;
        if (second.data[h2(key)].used && second.data[h2(key)].key == key) {
            value = second.data[h2(key)].value;
            return true;
        }
        return false;
    }

    bool place(const Entry& entry) {
        Entry current = entry;
        bool inFirst = true;
        int limit = first.size * 2;
        for (int i = 0; i < limit; ++i) {
            ++attempts;
            int index = inFirst ? h1(current.key) : h2(current.key);
            Entry& slot = inFirst ? first.data[index] : second.data[index];
            if (!slot.used) {
                slot = current;
                return true;
            }
            ++displacements;
            swap(slot, current);
            inFirst = !inFirst;
        }
        return false;
    }

    void rebuild(Array<Entry>& entries) {
        int capacity = first.size;
        int failures = 0;
        while (true) {
            first.clear();
            second.clear();
            Init(capacity);
            bool ok = true;
            for (int i = 0; i < entries.size; ++i) {
                if (!place(entries.data[i])) {
                    ok = false;
                    break;
                }
            }
            if (ok)
                return;
            ++rebuildAttempts;
            ++seed1;
            seed2 += 2;
            ++failures;
            if (failures % 5 == 0)
                capacity *= 2;
        }
    }

    void set(const string& key, const string& value) {
        string oldValue;
        if (get(key, oldValue)) {
            int a = h1(key), b = h2(key);
            if (first.data[a].used && first.data[a].key == key)
                first.data[a].value = value;
            else
                second.data[b].value = value;
            return;
        }

        Array<Entry> entries;
        entries.Init();
        for (int i = 0; i < first.size; ++i)
            if (first.data[i].used)
                entries.MPush(first.data[i]);
        for (int i = 0; i < second.size; ++i)
            if (second.data[i].used)
                entries.MPush(second.data[i]);
        entries.MPush(Entry{key, value, true});
        rebuild(entries);
        entries.clear();
    }

    bool contains(const string& key) {
        string value;
        return get(key, value);
    }

    void clear() {
        first.clear();
        second.clear();
    }
};
