#pragma once

#include <chrono>
#include <random>

#include "LongInt.hpp"

std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

// uniformly (almost) random number from 0 to n - 1 inclusive
LongInt RandUnder(LongInt n) {
    int lg = 0;
    {
        LongInt x = n;
        while (x > 0) {
            x >>= 1;
            ++lg;
        }
    }

    LongInt res = 0;
    for (int i = 0; i < 3 * lg / 32 + 5; ++i) {
        res = ((res << 32) + rng()) % n;
    }

    return res;
}
