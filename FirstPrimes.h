#pragma once

#include <vector>
#include <cassert>

namespace prime {
    class FirstPrimes {
    private:
        std::vector <bool> sieve_;
        std::vector <int> list_;
        int bound_ = 0;

    public:
        void RecomputeUpTo(int n) {
            if (bound_ >= n) {
                return;
            }

            bound_ = n;
            sieve_.assign((n - 2) / 2, true);
            list_ = {2};
            for (int i = 0; i < sieve_.size(); ++i) {
                if (sieve_[i]) {
                    list_.push_back(2 * i + 3);
                    for (int j = 2 * i * i + 6 * i + 3; j < sieve_.size(); j += 2 * i + 3) {
                        sieve_[j] = false;
                    }
                }
            }
        }

        bool IsPrime(int n) const {
            if (n <= 1) {
                return false;
            } else if (n == 2) {
                return true;
            } else if (n % 2 == 0) {
                return false;
            }
            assert(n < bound_);
            return sieve_[(n - 3) / 2];
        }

        // get n-th first prime
        size_t Get(size_t n) const {
            assert(n < list_.size());
            return list_[n];
        }

        size_t Count() const {
            return list_.size();
        }

        size_t Bound() const {
            return bound_;
        }
    };
    extern FirstPrimes first_primes;
};
