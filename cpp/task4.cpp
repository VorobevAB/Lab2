#include "task4.h"
#include "structures/array.h"
#include <iostream>
#include <string>
using namespace std;

void task4() {
    Array<int> count;
    count.Init();
    for (int i = 0; i < 256; ++i)
        count.MPush(0);

    string line;
    if (getline(cin, line)) {
        for (char c : line) {
            unsigned char value = static_cast<unsigned char>(c);
            if (value != ' ' && value != '\n')
                ++count.data[value];
        }
    }

    int maximum = 0;
    for (int i = 0; i < 256; ++i)
        if (count.data[i] > maximum)
            maximum = count.data[i];

    if (maximum == 0) {
        count.clear();
        return;
    }

    for (int row = maximum; row > 0; --row) {
        for (int i = 0; i < 256; ++i)
            if (count.data[i] > 0)
                cout << (count.data[i] >= row ? '#' : ' ');
        cout << endl;
    }
    
    for (int i = 0; i < 256; ++i)
        if (count.data[i] > 0)
            cout << static_cast<char>(i);
    cout << endl;
    
    count.clear();
}
