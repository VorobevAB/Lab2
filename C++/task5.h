#pragma once

struct BSTNode {
    int value;
    BSTNode* left = nullptr;
    BSTNode* right = nullptr;
};

void insertBST(BSTNode*& root, int v);
void printLeaves(BSTNode* root);
void clearBST(BSTNode* root);
void task5();
