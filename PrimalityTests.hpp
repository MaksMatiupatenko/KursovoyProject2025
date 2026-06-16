#pragma once

#include "LongInt.hpp"
#include "ModularMath.hpp"

#include <utility>
#include <stdexcept>

namespace prime {
    enum class PrimalityStatus {
        PROVED_PRIME = 2,
        PROBABLY_PRIME = 1,
        UNKNOWN = 0,
        PROBABLY_COMPOSITE = -1,
        PROVED_COMPOSITE = -2
    };

    // bruteforce test
    PrimalityStatus TestPrimalitySimple(const LongInt& n) {
        if (n <= 1) {
            return PrimalityStatus::PROVED_COMPOSITE;
        }
        for (LongInt i = 2; i * i <= n; ++i) {
            if (n % i == 0) {
                return PrimalityStatus::PROVED_COMPOSITE;
            }
        }
        return PrimalityStatus::PROVED_PRIME;
    }

    // returns pair {x, a}, that satisfies n == b^a * x, x % b != 0
    std::pair <LongInt, int> Split(LongInt n, int b) {
        assert(n >= 1);
        int a = 0;
        while (n % b == 0) {
            n /= b;
            ++a;
        }
        return {n, a};
    }
    std::pair <LongInt, int> Split(LongInt n, const LongInt& b) {
        assert(n >= 1);
        int a = 0;
        while (n % b == 0) {
            n /= b;
            ++a;
        }
        return {n, a};
    }

    // Miller-Rabin (strong pseudoprime) primality test
    PrimalityStatus TestPrimalityMillerRabin(LongInt n, int precision = 200) {
        if (n <= 1) {
            return PrimalityStatus::PROVED_COMPOSITE;
        } else if (n <= 3) {
            return PrimalityStatus::PROVED_PRIME;
        } else if (n % 2 == 0) {
            return PrimalityStatus::PROVED_COMPOSITE;
        }

        auto [x, a] = Split(n - 1, 2);

        for (LongInt iter = 0; iter < precision; ++iter) {
            LongInt base = random.uniform(2, n - 1);

            if (Gcd(n, base) != 1) {
                return PrimalityStatus::PROVED_COMPOSITE;
            }

            LongInt t = PowMod(base, x, n);
            if (t == 1 || t == n - 1) {
                continue;
            }
            int passed = 0;
            for (LongInt i = 0; i < a - 1; ++i) {
                t = t * t % n;
                if (t == n - 1) {
                    passed = 1;
                    break;
                }
            }
            if (!passed) {
                return PrimalityStatus::PROVED_COMPOSITE;
            }
        }
        return PrimalityStatus::PROBABLY_PRIME;
    }
};
