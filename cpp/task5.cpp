#include "task5.h"
#include "structures/bst.h"
#include <iostream>
using namespace std;

void task5() {
    BST tree;
    int value;
    while (cin >> value && value != 0)
        tree.insert(value);
    tree.printLeaves(tree.root);
    cout << endl;
    tree.clear();
}
