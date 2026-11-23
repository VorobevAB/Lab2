#include "task6.h"
#include "structures/hash_table.h"
#include <iostream>
#include <chrono>
#include <algorithm>
using namespace std;

bool isomorphicChain(const string& a, const string& b) {
    if (a.size() != b.size())
        return false;
    ChainHashMap<string> forward, backward;
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        string x(1, a[i]), y(1, b[i]), mapped;
        if (forward.get(x, mapped)) {
            if (mapped != y) { forward.clear(); backward.clear(); return false; }
        } else forward.set(x, y);

        if (backward.get(y, mapped)) {
            if (mapped != x) { forward.clear(); backward.clear(); return false; }
        } else backward.set(y, x);
    }
    forward.clear();
    backward.clear();
    return true;
}

bool isomorphicCuckoo(const string& a, const string& b) {
    if (a.size() != b.size())
        return false;
    CuckooHashMap forward, backward;
    forward.Init(16);
    backward.Init(16);
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        string x(1, a[i]), y(1, b[i]), mapped;
        if (forward.get(x, mapped)) {
            if (mapped != y) { forward.clear(); backward.clear(); return false; }
        } else forward.set(x, y);

        if (backward.get(y, mapped)) {
            if (mapped != x) { forward.clear(); backward.clear(); return false; }
        } else backward.set(y, x);
    }
    forward.clear();
    backward.clear();
    return true;
}

void task6() {
    int n;
    cin >> n;
    ChainHashMap<int> chain;
    CuckooHashMap cuckoo;
    cuckoo.Init(max(16, n));
    for (int i = 0; i < n; ++i) {
        string key;
        cin >> key;
        chain.set(key, i);
        cuckoo.set(key, to_string(i));
    }

    string check, cuckooValue;
    int chainValue = 0;
    cin >> check;
    auto start = chrono::steady_clock::now();
    bool chainFound = chain.get(check, chainValue);
    auto chainTime = chrono::duration_cast<chrono::nanoseconds>(
        chrono::steady_clock::now() - start).count();

    start = chrono::steady_clock::now();
    bool cuckooFound = cuckoo.get(check, cuckooValue);
    auto cuckooTime = chrono::duration_cast<chrono::nanoseconds>(
        chrono::steady_clock::now() - start).count();

    cout << "Chain: " << (chainFound ? "FOUND" : "NOT FOUND") << endl;
    cout << "Cuckoo: " << (cuckooFound ? "FOUND" : "NOT FOUND") << endl;

    string a, b;
    cin >> a >> b;
    cout << (isomorphicChain(a, b) ? "Chain isomorphic: YES" : "Chain isomorphic: NO") << endl;
    cout << (isomorphicCuckoo(a, b) ? "Cuckoo isomorphic: YES" : "Cuckoo isomorphic: NO") << endl;
    cout << "Chain find: " << chainTime << " ns; comparisons: " << chain.attempts << endl;
    cout << "Cuckoo find: " << cuckooTime << " ns; cell checks: " << cuckoo.lookupChecks << endl;

    chain.clear();
    cuckoo.clear();
}
