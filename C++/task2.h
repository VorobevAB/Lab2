#pragma once
#include "structures.h"
#include <string>

using namespace std;

struct ChainHash {
    static const int CAP = 101;
    ForwardList<string> table[CAP];
    unsigned long long attempts = 0;

    int h(const string& s) const;
    void add(const string& s);
    void del(const string& s);          
    bool contains(const string& s);
    void clear();
};

void task2(int argc, char** argv);
