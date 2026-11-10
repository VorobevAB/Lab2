#include "task4.h"
#include <iostream>

using namespace std;

void task4() {
    int count[256] = {};
    char c;

    while (cin.get(c)) {
        unsigned char x = (unsigned char)c;
        if (x != ' ' && x != '\n')
            ++count[x];
    }

    int mx = 0;
    for (int i = 0; i < 256; ++i)
        if (count[i] > mx)
            mx = count[i];

    for (int row = mx; row > 0; --row) {
        for (int i = 0; i < 256; ++i)
            if (count[i] > 0)
                cout << (count[i] >= row ? '#' : ' ');
        cout << endl;
    }

    for (int i = 0; i < 256; ++i)
        if (count[i] > 0)
            cout << (char)i;
    cout << endl;
}
