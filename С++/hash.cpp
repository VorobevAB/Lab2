#include "hash.h"

size_t hashString(const string& s) {
    size_t h = 0;
    for (unsigned char c : s)
        h = h * 31 + c;
    return h;
}
