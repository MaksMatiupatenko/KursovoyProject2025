#pragma once

#include <chrono>
#include <random>
#include <stdexcept>

#include "LongInt.hpp"

namespace prime {
    class Random {
    public:
        LongInt uniform(const LongInt& r) {
            if (r <= 0) {
                throw std::runtime_error("Uniform random range must have positive length");
            }

            int lg = 0;
            {
                LongInt x = r;
                while (x >= (1ll << 31)) {
                    x >>= 32;
                    lg += 32;
                }
                while (x > 0) {
                    x >>= 1;
                    ++lg;
                }
            }

            while (true) {
                LongInt res = 0;
                int curLen = 0;
                while (curLen + 32 <= lg) {
                    res = (res << 32) + generator_();
                    curLen += 32;
                }
                res = (res << (lg - curLen)) + generator_() % (1ll << (lg - curLen));
                if (res < r) {
                    return res;
                }
            }
        }

        LongInt uniform(const LongInt& l, const LongInt& r) {
            return l + uniform(r - l);
        }

    private:
        std::mt19937 generator_{52};
    };

    extern Random random;
};
