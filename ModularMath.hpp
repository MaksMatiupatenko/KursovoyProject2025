#pragma once

#include "LongInt.hpp"

// a^n % m
LongInt PowMod(LongInt a, LongInt n, LongInt m) {
    LongInt res = 1;
    while (n > 0) {
        if (n & 1) {
            res = res * a % m;
        }
        a = a * a % m;
        n >>= 1;
    }
    return res;
}
