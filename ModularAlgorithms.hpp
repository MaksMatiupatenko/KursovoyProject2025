#pragma once

#include "Poly.hpp"

namespace prime {
    int LegendreSymbol(int n, int p) {
        if (p == 2) {
            return n % p;
        }
        int res = PowMod(n % p, (p - 1) / 2, p);
        if (res == p - 1) {
            return -1;
        } else {
            return res;
        }
    }

    int LegendreSymbol(const LongInt& n, int p) {
        return LegendreSymbol((int)(n % p), p);
    }

    int ModSqrt(int n, int p) {
        n %= p;
        if (n == 0) {
            return 0;
        } else if (p == 2) {
            return n;
        } else if (p % 4 == 3) {
            return PowMod(n, (p + 1) / 4, p);
        }

        int a;
        do {
            a = (int)random.uniform(p);
        } while (LegendreSymbol((((long long)a * a - n) % p + p) % p, p) != -1);

        int b = ((n - (long long)a * a) % p + p) % p;
        Poly poly = PowMod(Poly(p, {a, 1}), (p + 1) / 2, Poly(p, {b, 0, 1}));
        int res = poly[0];
        assert((long long)res * res % p == n);
        return res;
    }

    int ModSqrt(const LongInt& n, int p) {
        return ModSqrt((int)(n % p), p);
    }

    // sqrt mod p^a
    LongInt ModSqrt(const LongInt& n, int p, int a) {
        if (a == 1) {
            return ModSqrt(n, p);
        }
        assert(p > 2);
        assert(a > 0);

        LongInt t = ModSqrt(n, p, a - 1);
        LongInt ppw = 1;
        for (int i = 0; i < a - 1; ++i) {
            ppw *= p;
        }
        int k = ((int)((n - t * t) / ppw % p) + p) % p;
        k = (long long)k * PowMod((int)(2 * t % p), p - 2, p) % p;
        return t + ppw * k;
    }
};