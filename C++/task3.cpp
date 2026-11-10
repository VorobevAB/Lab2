#include "task3.h"
#include "structures.h"
#include <iostream>

using namespace std;

struct Position {
    int a;
    int b;
};

void task3() {
    int n;
    if (!(cin >> n)) {
        cout << "ОШИБКА: ожидалось n" << endl;
        return;
    }
    if (n <= 0) {
        cout << "ОШИБКА: n > 0" << endl;
        return;
    }

    Array<Position> positions;
    positions.Init();

    for (int i = 0; i < n; ++i) {
        Position p;
        if (!(cin >> p.a >> p.b)) {
            cout << "ОШИБКА: ожидались a b для черепахи " << i + 1 << endl;
            positions.clear();
            return;
        }

        if (p.a < 0 || p.b < 0 || p.a > n - 1 || p.b > n - 1 || p.a + p.b != n - 1)
            continue;

        bool found = false;
        for (int j = 0; j < positions.size; ++j)
            if (positions.data[j].a == p.a && positions.data[j].b == p.b)
                found = true;

        if (!found)
            positions.MPush(p);
    }

    cout << positions.size << endl;
    positions.clear();
}
