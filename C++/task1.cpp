#include "task1.h"
#include "structures.h"
#include <iostream>

using namespace std;

void task1() {
    int n, m;
    if (!(cin >> n >> m)) {
        cout << "ОШИБКА: ожидалось n m" << endl;
        return;
    }
    if (n <= 0 || m < 0) {
        cout << "ОШИБКА: n > 0, m >= 0" << endl;
        return;
    }

    ForwardList<int>* g = new ForwardList<int>[n];
    for (int i = 0; i < m; ++i) {
        int a, b;
        if (!(cin >> a >> b)) {
            cout << "ОШИБКА: ожидалось ребро a b" << endl;
            for (int j = 0; j < n; ++j) g[j].clear();
            delete[] g;
            return;
        }
        if (a < 1 || a > n || b < 1 || b > n) {
            cout << "ОШИБКА: вершина вне [1.." << n << "]" << endl;
            for (int j = 0; j < n; ++j) g[j].clear();
            delete[] g;
            return;
        }
        --a; --b;
        g[a].FPushFront(b);
        g[b].FPushFront(a);
    }

    int start;
    if (!(cin >> start)) {
        cout << "ОШИБКА: ожидалась стартовая вершина" << endl;
        for (int j = 0; j < n; ++j) g[j].clear();
        delete[] g;
        return;
    }
    if (start < 1 || start > n) {
        cout << "ОШИБКА: старт вне [1.." << n << "]" << endl;
        for (int j = 0; j < n; ++j) g[j].clear();
        delete[] g;
        return;
    }
    --start;

    bool* used = new bool[n] {};
    Stack<int> st;
    st.SPush(start);

    while (!st.SEmpty()) {
        int v = st.STop();
        st.SPop();

        if (used[v])
            continue;

        used[v] = true;
        cout << v + 1 << " ";

        for (Node<int>* cur = g[v].head; cur; cur = cur->next)
            if (!used[cur->value])
                st.SPush(cur->value);
    }
    cout << endl;

    st.clear();
    for (int i = 0; i < n; ++i) g[i].clear();
    delete[] g;
    delete[] used;
}
