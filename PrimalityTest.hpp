#pragma once

#include "LongInt.hpp"
#include "ModularMath.hpp"
#include "FirstPrimes.hpp"

// bruteforce test
bool SimplePrimalityTest(LongInt n) {
    if (n <= 1) {
        return false;
    }
    for (LongInt i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

// Miller-Rabin (strong pseudoprime) primality test
bool MillerRabinPrimalityTest(LongInt n) {
    if (n <= 1) {
        return false;
    }

    LongInt small_primes[] = { 2, 3, 5, 7, 11, 13, 17, 19 };
    for (auto p : small_primes) {
        if (p < n && n % p == 0) {
            return false;
        }
        if (p == n) {
            return true;
        }
    }

    // n == 2^a * x + 1, x % 2 == 1
    LongInt x = n - 1;
    LongInt a = 0;
    while (x % 2 == 0) {
        x /= 2;
        ++a;
    }

    auto test_base = [&](LongInt base) {
        if (base % n == 0) {
            return true;
        }
        LongInt t = PowMod(base, x, n);
        if ((t - 1) % n == 0) {
            return true;
        }
        for (LongInt i = 0; i < a; ++i) {
            if ((t + 1) % n == 0) {
                return true;
            }
            t = t * t % n;
        }
        return false;
        };

    for (LongInt iter = 0; iter < 500; ++iter) {
        if (!test_base(RandUnder(n))) {
            return false;
        }
    }
    return true;
}
