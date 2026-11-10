#include "task6.h"
#include "task2.h"
#include "hash.h"
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <algorithm>

using namespace std;

CuckooHash::CuckooHash(int n) {
    initTables(n);
}

void CuckooHash::initTables(int n) {
    t1.Init();
    t2.Init();
    for (int i = 0; i < n; ++i) {
        t1.MPush("");
        t2.MPush("");
    }
}

int CuckooHash::h1(const string& s) const {
    return (int)((hashString(s) + seed1) % t1.size);
}

int CuckooHash::h2(const string& s) const {
    return (int)((hashString(s) * 33 + seed2) % t2.size);
}

bool CuckooHash::contains(const string& s) const {
    ++lookupChecks;
    if (t1.data[h1(s)] == s) return true;
    ++lookupChecks;
    return t2.data[h2(s)] == s;
}

bool CuckooHash::place(const string& s) {
    string cur = s;
    bool inFirst = true;
    const int MAX_LOOP = 2 * t1.size;

    for (int i = 0; i < MAX_LOOP; ++i) {
        ++attempts;
        if (inFirst) {
            int p = h1(cur);
            if (!t1.data[p].empty()) ++displacements;
            swap(cur, t1.data[p]);
            if (cur.empty())
                return true;
        }
        else {
            int p = h2(cur);
            if (!t2.data[p].empty()) ++displacements;
            swap(cur, t2.data[p]);
            if (cur.empty())
                return true;
        }
        inFirst = !inFirst;
    }
    return false;
}

void CuckooHash::rebuild(Array<string>& keys) {
    int cap = t1.size;
    int failedAttempts = 0;

    while (true) {
        t1.clear();
        t2.clear();
        initTables(cap);

        bool ok = true;
        for (int i = 0; i < keys.size; ++i) {
            if (!place(keys.data[i])) {
                ok = false;
                break;
            }
        }

        if (ok)
            return;

        ++rebuildAttempts;
        ++seed1;
        seed2 += 2;
        ++failedAttempts;

        if (failedAttempts % 5 == 0)
            cap *= 2;
    }
}

void CuckooHash::add(const string& s) {
    if (s.empty() || contains(s))
        return;
    Array<string> keys;
    keys.Init();

    for (int i = 0; i < t1.size; ++i)
        if (!t1.data[i].empty())
            keys.MPush(t1.data[i]);
    for (int i = 0; i < t2.size; ++i)
        if (!t2.data[i].empty())
            keys.MPush(t2.data[i]);
    keys.MPush(s);

    rebuild(keys);
    keys.clear();
}

void CuckooHash::clear() {
    t1.clear();
    t2.clear();
}

bool isomorphic(const string& a, const string& b) {
    if (a.size() != b.size())
        return false;

    int ab[256];
    int ba[256];
    for (int i = 0; i < 256; ++i) {
        ab[i] = -1;
        ba[i] = -1;
    }

    for (int i = 0; i < (int)a.size(); ++i) {
        unsigned char x = (unsigned char)a[i];
        unsigned char y = (unsigned char)b[i];

        if (ab[x] != -1 && ab[x] != y)
            return false;
        if (ba[y] != -1 && ba[y] != x)
            return false;

        ab[x] = y;
        ba[y] = x;
    }
    return true;
}

void task6() {
    using clock_type = chrono::steady_clock;
    int n;
    if (!(cin >> n)) {
        cout << "ОШИБКА: ожидалось n" << endl;
        return;
    }
    if (n <= 0) {
        cout << "ОШИБКА: n > 0" << endl;
        return;
    }

    ChainHash chain;
    CuckooHash cuckoo;
    auto insertStart = clock_type::now();
    for (int i = 0; i < n; ++i) {
        string key;
        if (!(cin >> key)) {
            cout << "ОШИБКА: ожидался ключ #" << i + 1 << endl;
            chain.clear(); cuckoo.clear(); return;
        }
        if (key.empty()) {
            cout << "ОШИБКА: пустой ключ" << endl;
            chain.clear(); cuckoo.clear(); return;
        }
        chain.add(key);
        cuckoo.add(key);
    }
    auto insertTime = chrono::duration_cast<chrono::nanoseconds>(clock_type::now() - insertStart).count();

    string check;
    if (!(cin >> check)) {
        cout << "ОШИБКА: ожидался ключ для поиска" << endl;
        chain.clear(); cuckoo.clear(); return;
    }
    auto findStart = clock_type::now();
    bool chainFound = chain.contains(check);
    auto chainFindTime = chrono::duration_cast<chrono::nanoseconds>(clock_type::now() - findStart).count();
    findStart = clock_type::now();
    bool cuckooFound = cuckoo.contains(check);
    auto cuckooFindTime = chrono::duration_cast<chrono::nanoseconds>(clock_type::now() - findStart).count();

    cout << endl << "Chain: " << (chainFound ? "FOUND" : "NOT FOUND") << endl;
    cout << "Cuckoo: " << (cuckooFound ? "FOUND" : "NOT FOUND") << endl;

    string a, b;
    if (!(cin >> a >> b)) {
        cout << "ОШИБКА: ожидались две строки для проверки изоморфизма" << endl;
        chain.clear(); cuckoo.clear(); return;
    }
    auto isoStart = clock_type::now();
    bool isoResult = isomorphic(a, b);
    auto isoTime = chrono::duration_cast<chrono::nanoseconds>(clock_type::now() - isoStart).count();
    cout << (isoResult ? "YES" : "NO") << endl;

    cout << endl << "Статистика выполнения:" << endl;
    cout << "Добавление " << n << " ключей в обе таблицы: " << insertTime << " нс" << endl;
    cout << "Поиск в Chain Hash: " << chainFindTime << " нс; сравнений: " << chain.attempts << endl;
    cout << "Поиск в Cuckoo Hash: " << cuckooFindTime << " нс; проверок ячеек: " << cuckoo.lookupChecks << endl;
    cout << "Cuckoo Hash: попыток размещения: " << cuckoo.attempts
         << "; вытеснений: " << cuckoo.displacements
         << "; перестроек: " << cuckoo.rebuildAttempts << endl;
    cout << "Проверка изоморфизма: " << isoTime << " нс" << endl;

    chain.clear();
    cuckoo.clear();

    using clock_type = chrono::steady_clock;
    const int sizes[] = {100, 1000, 5000};
    cout << endl << "Эмпирический анализ (время в микросекундах)" << endl;
    cout << "N\tChain insert\tChain find\tChain comparisons\tCuckoo insert\tCuckoo find\tCuckoo placements\tDisplacements\tRebuilds\n";

    for (int n : sizes) {
        ChainHash chain;
        CuckooHash cuckoo( max(16, n) );
        Array<string> keys; keys.Init();
        for (int i = 0; i < n; ++i) keys.MPush("key_" + to_string(i));

        auto start = clock_type::now();
        for (int i = 0; i < n; ++i) chain.add(keys.data[i]);
        auto chainInsert = chrono::duration_cast<chrono::microseconds>(clock_type::now() - start).count();
        start = clock_type::now();
        for (int i = 0; i < n; ++i) chain.contains(keys.data[i]);
        auto chainFind = chrono::duration_cast<chrono::microseconds>(clock_type::now() - start).count();
        auto chainComparisons = chain.attempts;

        start = clock_type::now();
        for (int i = 0; i < n; ++i) cuckoo.add(keys.data[i]);
        auto cuckooInsert = chrono::duration_cast<chrono::microseconds>(clock_type::now() - start).count();
        start = clock_type::now();
        volatile int found = 0;
        for (int i = 0; i < n; ++i) if (cuckoo.contains(keys.data[i])) ++found;
        auto cuckooFind = chrono::duration_cast<chrono::microseconds>(clock_type::now() - start).count();

        cout << n << '\t' << chainInsert << '\t' << chainFind << '\t' << chainComparisons
             << '\t' << cuckooInsert << '\t' << cuckooFind << '\t' << cuckoo.attempts
             << '\t' << cuckoo.displacements << '\t' << cuckoo.rebuildAttempts << endl;
        keys.clear(); chain.clear(); cuckoo.clear();
    }

}
