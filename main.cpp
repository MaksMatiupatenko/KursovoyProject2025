#include "LongInt.hpp"
#include "ModularMath.hpp"
#include "Random.h"
#include "PrimalityTests.hpp"
#include "Factorization.hpp"

#include <iostream>
#include <vector>


namespace {
    prime::LongInt FindBiggerPrime(prime::LongInt n) {
        while (prime::TestPrimalityMillerRabin(n) < prime::PrimalityStatus::PROBABLY_PRIME) {
            ++n;
        }
        return n;
    }
    prime::LongInt FindNDigitPrime(int d) {
        prime::LongInt pw = 1;
        for (int i = 0; i < d - 1; ++i) {
            pw *= 10;
        }
        while (true) {
            prime::LongInt n = prime::random.uniform(pw, pw * 10);
            if (prime::TestPrimalityMillerRabin(n) >= prime::PrimalityStatus::PROBABLY_PRIME) {
                return n;
            }
        }
    }
};


int main() {
    while (true) {
        int len, cnt;
        // std::cin >> len >> cnt;
        // len = (int)prime::random.uniform(8, 12);
        // cnt = (int)prime::random.uniform(2, 3);
        len = 13;
        cnt = 2;
        prime::LongInt num = 1;
        for (int i = 0; i < cnt; ++i) {
            num *= ::FindNDigitPrime(len);
        }
        std::cout << num << '\n';
        prime::Clock clock{};
        // auto res = prime::FactorizePollardRho(num);
        auto res = prime::FactorizeQuadraticSieve(num, 200000);
        for (const auto& x : res) {
            std::cout << x << '\n';
        }
        std::cout << "Time: " << clock.Time() << " seconds\n";
    }
}
