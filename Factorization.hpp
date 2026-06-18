#pragma once

#include "LongInt.hpp"
#include "PrimalityTests.hpp"
#include "FirstPrimes.h"
#include "Random.h"
#include "ModularAlgorithms.hpp"
#include "bitset.h"
#include <algorithm>
#include <vector>
#include <queue>

#include <iostream>
#include "Clock.hpp"

namespace prime {
    std::vector<LongInt> FactorizeSimple(LongInt n) {
        std::vector<LongInt> res;
        for (LongInt i = 2; i * i <= n; ++i) {
            auto [x, a] = Split(n, i);
            n = x;
            res.resize(res.size() + a, i);
        }
        if (n != 1) {
            res.push_back(n);
        }
        return res;
    }

    LongInt FindDivisorPollardRho(LongInt n, long long max_iter) {
        LongInt c = random.uniform(n);
        auto f = [&c](LongInt x) {
            return x * x + c;
        };

        if (max_iter == 0) {
            max_iter = (1ll << 62);
        } else if (max_iter == -1) {
            if (n >= 1'000'000'000) {
                max_iter = (long long)(10 * Sqrt(Sqrt(n)));
            } else {
                max_iter = 100000;
            }
        }

        LongInt x1 = 0;
        LongInt x2 = x1;
        int iter = 0;
        while (iter < max_iter) {
            ++iter;
            LongInt g = Gcd(Abs(x1 - x2), n);
            if (g != 1 && g != n) {
                return g;
            }
            x1 = f(x1) % n;
            x2 = f(f(x2)) % n;
        }

        return n;
    }
    void RecursiveFactorizePollardRho(LongInt n,
                                      std::vector<LongInt>* out,
                                      long long max_iter) {

        while (n != 1 && TestPrimalityMillerRabin(n) == PrimalityStatus::PROVED_COMPOSITE) {
            LongInt d = FindDivisorPollardRho(n, max_iter);
            if (d != n) {
                RecursiveFactorizePollardRho(d, out, max_iter);
                n /= d;
            }
        }
        if (n != 1) {
            out->push_back(n);
        }
    }

    std::pair<std::vector<LongInt>, LongInt> FactorizeUpToBound(LongInt n,
                                                                const LongInt& bound) {

        first_primes.RecomputeUpTo((int)bound);
        std::vector<LongInt> res;
        for (int i = 0; i < first_primes.Count() && first_primes.Get(i) < bound; ++i) {
            auto [x, a] = Split(n, first_primes.Get(i));
            n = x;
            res.resize(res.size() + a, first_primes.Get(i));
        }

        return {res, n};
    }

    std::pair<std::vector<int>, LongInt> FactorizeByBase(LongInt n,
                                                         const std::vector<int>& base) {

        std::vector<int> res(base.size());
        for (int i = 0; i < base.size(); ++i) {
            auto [x, a] = Split(n, base[i]);
            n = x;
            res[i] = a;
        }

        return {res, n};
    }

    std::vector<LongInt> FactorizePollardRho(LongInt n,
                                             long long max_iter = -1,
                                             size_t small_primes_bound = 100000) {

        auto [res, x] = FactorizeUpToBound(n, small_primes_bound);
        RecursiveFactorizePollardRho(x, &res, max_iter);
        std::sort(res.begin(), res.end());
        return res;
    }

    LongInt FindDivisorQuadraticSieve(LongInt n,
                                      int factorBaseBound,
                                      int oversampling,
                                      int mx_prime_power) {
        Clock clock{};
        std::vector<int> factorBase;
        for (int i = 0; i < first_primes.Count() && first_primes.Get(i) < factorBaseBound; ++i) {
            if (LegendreSymbol(n, first_primes.Get(i)) == 1 && first_primes.Get(i) != 2) {
                factorBase.push_back(first_primes.Get(i));
            }
        }

        std::vector<std::vector<LongInt>> sqRoot;
        std::vector<std::vector<LongInt>> factorBasePw;
        std::vector<double> fb_lg;
        for (auto p : factorBase) {
            factorBasePw.push_back({});
            sqRoot.push_back({});
            LongInt ppw = 1;
            for (int i = 0; i < mx_prime_power; ++i) {
                ppw *= p;
                factorBasePw.back().push_back(ppw);
                sqRoot.back().push_back(ModSqrt(n, p, i + 1));
            }
            fb_lg.push_back(Log(p));
        }

        LongInt r = Sqrt(n) + 1;
        LongInt r2 = Sqrt(2 * n);
        std::priority_queue<std::tuple<LongInt, int, int>,
                            std::vector<std::tuple<LongInt, int, int>>,
                            std::greater<std::tuple<LongInt, int, int>>> schedule;
        for (int i = 0; i < factorBase.size(); ++i) {
            LongInt ppw = 1;
            int p = factorBase[i];
            for (int j = 0; j < mx_prime_power; ++j) {
                ppw *= p;
                LongInt nx = r / ppw * ppw + sqRoot[i][j];
                if (nx >= r) {
                    schedule.emplace(nx, i + 1, j + 1);
                } else {
                    schedule.emplace(nx + ppw, i + 1, j + 1);
                }
                
                nx = r / ppw * ppw + ppw - sqRoot[i][j];
                if (nx >= r) {
                    schedule.emplace(nx, i + 1, j + 1);
                } else {
                    schedule.emplace(nx + ppw, i + 1, j + 1);
                }
            }
        }

        int fbSize = factorBase.size() + 1;
        int need_nums = fbSize + oversampling;
        std::vector<Bitset> matrix(fbSize, Bitset(need_nums));
        std::vector<LongInt> prod;
        std::vector<LongInt> source;
        long long checked = 0;
        while (r < r2 && prod.size() < need_nums) {
            ++checked;
            r = get<0>(schedule.top());
            auto [x, pw2] = Split(r * r - n, 2);
            std::vector<std::pair<int, int>> fact = {{0, pw2}};
            double log = Log(x);
            while (get<0>(schedule.top()) == r) {
                auto [t, ind, pw] = schedule.top();
                int p = factorBase[ind - 1];
                schedule.pop();
                schedule.emplace(t + factorBasePw[ind - 1][pw - 1], ind, pw);
                log -= fb_lg[ind - 1];
                fact.emplace_back(ind, pw);
            }
            if (log < 0.1) {
                for (auto [ind, pw] : fact) {
                    matrix[ind][prod.size()] = pw % 2;
                }
                prod.push_back(r * r - n);
                source.push_back(r);
            }
        }

        if (prod.size() < need_nums) {
            return FactorizeSimple(n)[0];
        }

        std::vector<int> dependRow(matrix[0].size());
        int row = 0;
        for (int i = 0; i < matrix[0].size(); ++i) {
            int j = row;
            while (j < matrix.size() && matrix[j][i] != 1) {
                ++j;
            }
            if (j == matrix.size()) {
                dependRow[i] = -1;
                continue;
            } else {
                dependRow[i] = row;
            }
            std::swap(matrix[row], matrix[j]);

            for (j = 0; j < matrix.size(); ++j) {
                if (j != row && matrix[j][i] == 1) {
                    matrix[j] ^= matrix[row];
                }
            }
            ++row;
        }

        while (true) {
            LongInt curSource = 1;
            std::vector<int> prodFact(fbSize);
            std::vector<bool> picked(matrix[0].size());
            std::vector<bool> curRes(matrix.size());

            for (int i = matrix[0].size() - 1; i >= 0; --i) {
                if (dependRow[i] == -1) {
                    picked[i] = (int)random.uniform(2);
                    if (picked[i]) {
                        for (int j = 0; j < matrix.size(); ++j) {
                            curRes[j] = curRes[j] ^ matrix[j][i];
                        }
                    }
                } else {
                    picked[i] = curRes[dependRow[i]];
                    curRes[dependRow[i]] = curRes[dependRow[i]] ^ picked[i];
                }

                if (picked[i]) {
                    curSource = curSource * source[i] % n;
                    auto [x, pw2] = Split(prod[i], 2);
                    auto [fact, resid] = FactorizeByBase(x, factorBase);
                    assert(resid == 1);
                    prodFact[0] += pw2;
                    for (int j = 0; j < factorBase.size(); ++j) {
                        prodFact[j + 1] += fact[j];
                    }
                }
            }

            assert(curRes[0] == 0);
            assert(prodFact[0] % 2 == 0);
            LongInt curProdSqrt = PowMod(2, prodFact[0] / 2, n);
            for (int j = 0; j < factorBase.size(); ++j) {
                assert(curRes[j + 1] == 0);
                assert(prodFact[j + 1] % 2 == 0);
                curProdSqrt *= PowMod(factorBase[j], prodFact[j + 1] / 2, n);
                curProdSqrt %= n;
            }
            assert(curSource * curSource % n == curProdSqrt * curProdSqrt % n);

            LongInt g = Gcd(n, curSource - curProdSqrt);
            if (g != 1 && g != n) {
                return g;
            }
        }
    }

    void RecursiveFactorizeQuadraticSieve(LongInt n,
                                          std::vector<LongInt>* out,
                                          int factorBaseBound,
                                          int oversampling,
                                          int mx_prime_power) {

        while (n != 1 && TestPrimalityMillerRabin(n) == PrimalityStatus::PROVED_COMPOSITE) {
            LongInt d = FindDivisorQuadraticSieve(n, factorBaseBound, oversampling, mx_prime_power);
            if (d != n) {
                RecursiveFactorizeQuadraticSieve(d, out, factorBaseBound, oversampling, mx_prime_power);
                n /= d;
            }
        }
        if (n != 1) {
            out->push_back(n);
        }
    }

    std::vector<LongInt> FactorizeQuadraticSieve(LongInt n,
                                                 int factorBaseBound = 200000,
                                                 int oversampling = 10,
                                                 int mx_prime_power = 4) {
        auto [res, x] = FactorizeUpToBound(n, factorBaseBound);
        RecursiveFactorizeQuadraticSieve(x, &res, factorBaseBound, oversampling, mx_prime_power);
        std::sort(res.begin(), res.end());
        return res;
    }
};
