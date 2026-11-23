#pragma once
#include <iostream>
using namespace std;

struct BST {
    struct Node {
        int value;
        Node* left;
        Node* right;
    };

    Node* root = nullptr;

    void insert(int value) {
        Node** current = &root;
        while (*current) {
            if (value < (*current)->value)
                current = &(*current)->left;
            else if (value > (*current)->value)
                current = &(*current)->right;
            else
                return;
        }
        *current = new Node{value, nullptr, nullptr};
    }

    void printLeaves(Node* node) const {
        if (!node)
            return;
        printLeaves(node->left);
        if (!node->left && !node->right)
            cout << node->value << " ";
        printLeaves(node->right);
    }

    void clearNode(Node* node) {
        if (!node)
            return;
        clearNode(node->left);
        clearNode(node->right);
        delete node;
    }

    void clear() {
        clearNode(root);
        root = nullptr;
    }
};
