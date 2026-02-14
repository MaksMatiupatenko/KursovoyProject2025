#pragma once

#include "LongInt.hpp"
#include "PrimalityTest.hpp"
#include "Random.hpp"
#include <algorithm>
#include <vector>

std::vector<LongInt> SimpleFactorization(LongInt n) {
    std::vector<LongInt> res;
    for (LongInt i = 2; i * i < n; ++i) {
        while (n % i == 0) {
            res.push_back(i);
            n /= i;
        }
    }
    if (n != 1) {
        res.push_back(n);
    }
    return res;
}

LongInt _PollardRhoFindDivisor(LongInt n, size_t max_iter = -1) {
    LongInt c = RandUnder(n);
    auto f = [&c](LongInt x) {
        return x * x + c;
    };

    LongInt x1 = 0;
    LongInt x2 = x1;
    size_t iter = 0;
    while (iter < max_iter) {
        ++iter;
        LongInt g = gcd(Abs(x1 - x2), n);
        if (g != 1 && g != n) {
            return g;
        }
        x1 = f(x1) % n;
        x2 = f(f(x2)) % n;
    }

    return n;
}
void _PollardRhoFactorization(LongInt n, std::vector<LongInt>& out) {
    while (n != 1 && !MillerRabinPrimalityTest(n)) {
        LongInt d = _PollardRhoFindDivisor(n, 100000);
        if (d != n) {
            _PollardRhoFactorization(d, out);
            n /= d;
        }
    }
    if (n != 1) {
        out.push_back(n);
    }
}
std::vector<LongInt> PollardRhoFactorization(LongInt n) {
    const int small_primes_bound = 100000;
    if (first_primes.GetBound() < small_primes_bound) {
        first_primes.init(small_primes_bound);
    }

    std::vector<LongInt> res;
    for (size_t i = 0; i < first_primes.cnt() && first_primes.get(i) < small_primes_bound; ++i) {
        while (n % first_primes.get(i) == 0) {
            res.push_back(first_primes.get(i));
            n /= first_primes.get(i);
        }
    }
    _PollardRhoFactorization(n, res);
    std::sort(res.begin(), res.end());
    return res;
}