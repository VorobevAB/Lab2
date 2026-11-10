#pragma once
#include "structures.h"

struct CuckooHash {
    Array<string> t1;
    Array<string> t2;
    size_t seed1 = 0;
    size_t seed2 = 1;
    unsigned long long attempts = 0;
    unsigned long long displacements = 0;
    unsigned long long rebuildAttempts = 0;
    mutable unsigned long long lookupChecks = 0;

    CuckooHash(int n = 16);
    void initTables(int n);
    int h1(const string& s) const;
    int h2(const string& s) const;
    bool contains(const string& s) const;
    bool place(const string& s);
    void rebuild(Array<string>& keys);
    void add(const string& s);
    void clear();
};

bool isomorphic(const string& a, const string& b);
void task6();
