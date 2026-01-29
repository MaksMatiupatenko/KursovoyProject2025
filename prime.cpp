#include <boost/multiprecision/cpp_int.hpp>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

using LongInt = boost::multiprecision::cpp_int;

std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

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

LongInt BiggerPrime(LongInt n) {
    while (!MillerRabinPrimalityTest(n)) {
        ++n;
    }
    return n;
}

LongInt NDigitPrime(int d) {
    LongInt pw = 1;
    for (int i = 0; i < d - 1; ++i) {
        pw *= 10;
    }
    return BiggerPrime(pw + RandUnder(pw * 8));
}

int main()
{
    if (true) {
        int n;
        std::cin >> n;
        std::cout << MillerRabinPrimalityTest(n) << '\n';
    }
}