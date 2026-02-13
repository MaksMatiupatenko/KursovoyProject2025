#pragma once

#include <stdexcept>
#include <vector>

class FirstPrimes {
private:
    std::vector <bool> sieve;
    std::vector <size_t> list;
    size_t bound = 0;

public:
    // find all primes less than n
    void init(size_t n) {
        bound = n;
        sieve.assign(n, 1);
        list.clear();
        sieve[0] = 0;
        sieve[1] = 0;
        for (size_t i = 2; i < n; ++i) {
            if (sieve[i]) {
                list.push_back(i);
                for (size_t j = i * i; j < n; j += i) {
                    sieve[j] = 0;
                }
            }
        }
    }

    bool IsPrime(size_t n) {
        if (bound <= n) {
            throw std::runtime_error("FirstPrimes out of bound");
        }
        return sieve[n];
    }

    size_t get(size_t ind) {
        if (list.size() <= ind) {
            throw std::runtime_error("FirstPrimes out of bound");
        }
        return list[ind];
    }

    size_t cnt() {
        return list.size();
    }
};
static FirstPrimes first_primes;