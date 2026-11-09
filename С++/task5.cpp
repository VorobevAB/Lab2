#include "task5.h"
#include <iostream>
#include <string>

using namespace std;

void insertBST(BSTNode*& root, int v) {
    if (!root) {
        root = new BSTNode{ v, nullptr, nullptr };
        return;
    }
    if (v < root->value)
        insertBST(root->left, v);
    else if (v > root->value)
        insertBST(root->right, v);
}

void printLeaves(BSTNode* root) {
    if (!root)
        return;
    printLeaves(root->left);
    if (!root->left && !root->right)
        cout << root->value << " ";
    printLeaves(root->right);
}

void clearBST(BSTNode* root) {
    if (!root)
        return;
    clearBST(root->left);
    clearBST(root->right);
    delete root;
}

void task5() {
    BSTNode* root = nullptr;

    string token;
    while (cin >> token) {
        bool isNum = !token.empty();
        int pos = 0;
        if (token[0] == '-') {
            if (token.size() == 1) { isNum = false; }
            else pos = 1;
        }
        for (; isNum && pos < (int)token.size(); ++pos)
            if (token[pos] < '0' || token[pos] > '9')
                isNum = false;

        if (!isNum) {
            cout << "ОШИБКА: '" << token << "' — не целое число" << endl;
            clearBST(root);
            return;
        }

        int x = stoi(token);
        if (x == 0)
            break;
        insertBST(root, x);
    }

    printLeaves(root);
    cout << endl;
    clearBST(root);
}
