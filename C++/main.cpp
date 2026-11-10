#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"
#include "task5.h"
#include "task6.h"
#include "task7.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Укажите номер задания: 1-7" << endl;
        return 0;
    }

    int task = argv[1][0] - '0';

    switch (task) {
    case 1: task1(); break;
    case 2: task2(argc, argv); break;
    case 3: task3(); break;
    case 4: task4(); break;
    case 5: task5(); break;
    case 6: task6(); break;
    case 7: task7(); break;
    default: cout << "Нет такого задания" << endl;
    }

    return 0;
}
