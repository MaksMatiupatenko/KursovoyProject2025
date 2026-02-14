#include "LongInt.hpp"
#include "ModularMath.hpp"
#include "Random.hpp"
#include "PrimalityTest.hpp"
#include "FirstPrimes.hpp"
#include "Factorization.hpp"

#include <iostream>
#include <vector>


// just fun, finding big primes by checking random numbers
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

int main() {
    while (true) {
        LongInt n;
        std::cin >> n;
        auto v = PollardRhoFactorization(n);
        // std::cout << v.size() << '\n';
        for (auto x : v) {
            std::cout << x << '\n';
        }
    }
}
