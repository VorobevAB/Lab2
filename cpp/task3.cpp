#include "task3.h"
#include "structures/hash_table.h"
#include <iostream>
using namespace std;

void task3() {
    int n;
    cin >> n;
    StringSet positions;
    int count = 0;
    for (int i = 0; i < n; ++i) {
        int ahead, behind;
        cin >> ahead >> behind;
        if (ahead < 0 || behind < 0 || ahead + behind != n - 1)
            continue;
        string key = to_string(ahead) + ":" + to_string(behind);
        if (!positions.contains(key)) {
            positions.add(key);
            ++count;
        }
    }
    cout << count << endl;
    positions.clear();
}
