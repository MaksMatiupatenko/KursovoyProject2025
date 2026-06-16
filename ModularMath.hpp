#pragma once

#include "LongInt.hpp"
#include <cmath>

namespace prime {
    // a^n % m
    LongInt PowMod(LongInt a, LongInt n, const LongInt& m) {
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

    int PowMod(int a, int n, int m) {
        int res = 1;
        while (n > 0) {
            if (n & 1) {
                res = (long long)res * a % m;
            }
            a = (long long)a * a % m;
            n >>= 1;
        }
        return res;
    }

    LongInt Abs(const LongInt& n) {
        return boost::multiprecision::abs(n);
    }

    LongInt Gcd(LongInt a, LongInt b) {
        a = Abs(a);
        b = Abs(b);
        while (b != 0) {
            LongInt tmp = a % b;
            a = b;
            b = tmp;
        }
        return a;
    }

    int Log2(LongInt n) {
        int res = 0;
        while (n > 0) {
            n >>= 1;
            ++res;
        }
        return res;
    }

    LongInt Sqrt(const LongInt& n) {
        return boost::multiprecision::sqrt(n);
    }

    double Log(const LongInt& n) {
        return std::log2((double)n);
    }
};
