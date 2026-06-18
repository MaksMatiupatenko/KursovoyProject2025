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
    prime::LongInt RandomNDigit(int d) {
        prime::LongInt pw = 1;
        for (int i = 0; i < d - 1; ++i) {
            pw *= 10;
        }
        return prime::random.uniform(pw, pw * 10);
    }
};


int main() {
    prime::Clock clock{};

    int len, alg, tests, type;
    std::cin >> len >> alg >> tests >> type;
    float sum = 0, mx = 0;
    for (int i = 0; i < tests; ++i) {
        prime::LongInt num = 1;
        if (type == 1) {
            for (int i = 0; i < 2; ++i) {
                num *= ::FindNDigitPrime(len / 2);
            }
        } else {
            num = ::RandomNDigit(len);
        }

        std::cout << num << '\n';
        
        
        std::vector<prime::LongInt> res;
        clock.Restart();
        if (alg == 1) {
            res = prime::FactorizeSimple(num);
        } else if (alg == 2) {
            res = prime::FactorizePollardRho(num);
        } else {
            res = prime::FactorizeQuadraticSieve(num);
        }
        float t = clock.Time();

        for (const auto& x : res) {
            std::cout << x << '\n';
        }

        sum += t;
        mx = std::max(mx, t);
    }
    
    std::cout << "Avg: " << sum / tests << " seconds\n";
    std::cout << "Max: " << mx << " seconds\n";
}
