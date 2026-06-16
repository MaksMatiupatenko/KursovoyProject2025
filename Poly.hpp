#pragma once

#include "ModularMath.hpp"

#include <vector>

namespace prime {
    // Polynom over Z_p
    class Poly {
    public:
        explicit Poly(int mod) : mod_(mod) {}
        Poly(int mod, int val) : mod_(mod), data_({(val % mod + mod) % mod}) {}
        Poly(int mod, const std::vector<int>& vals) : mod_(mod), data_(vals) {
            for (auto& x : data_) {
                x = (x % mod_ + mod_) % mod_;
            }
        }

        int operator[](int ind) const {
            if (ind >= data_.size()) {
                return 0;
            }
            return data_[ind];
        }

        Poly operator<<(int val) const {
            Poly res = *this;
            std::vector<int> toInsert(val, 0);
            res.data_.insert(res.data_.begin(), toInsert.begin(), toInsert.end());
            return res;
        }

        Poly operator-(const Poly& other) const {
            assert(mod_ == other.mod_);
            Poly res(mod_);
            res.data_.resize(std::max(data_.size(), other.data_.size()));
            for (int i = 0; i < res.data_.size(); ++i) {
                res.data_[i] = ((*this)[i] - other[i] + mod_) % mod_;
            }
            while (!res.data_.empty() && res.data_.back() == 0) {
                res.data_.pop_back();
            }
            return res;
        }

        Poly operator*(int k) const {
            Poly res = *this;
            for (int i = 0; i < data_.size(); ++i) {
                res.data_[i] = (long long)res.data_[i] * k % mod_;
            }
            while (!res.data_.empty() && res.data_.back() == 0) {
                res.data_.pop_back();
            }
            return res;
        }

        Poly operator*(const Poly& other) const {
            assert(mod_ == other.mod_);
            Poly res(mod_);
            res.data_.resize(data_.size() + other.data_.size());
            for (int i = 0; i < data_.size(); ++i) {
                for (int j = 0; j < other.data_.size(); ++j) {
                    res.data_[i + j] = (res.data_[i + j] + (long long)data_[i] * other.data_[j]) % mod_;
                }
            }
            while (!res.data_.empty() && res.data_.back() == 0) {
                res.data_.pop_back();
            }
            return res;
        }

        Poly operator%(const Poly& other) const {
            assert(mod_ == other.mod_);
            assert(!other.data_.empty());
            Poly res = *this;
            while (res.data_.size() >= other.data_.size()) {
                int k = ((long long)PowMod(other.data_.back(), mod_ - 2, mod_) * res.data_.back()) % mod_;
                res = res - ((other * k) << (res.data_.size() - other.data_.size()));
            }
            while (!res.data_.empty() && res.data_.back() == 0) {
                res.data_.pop_back();
            }
            return res;
        }

        int Mod() const {
            return mod_;
        }

    private:
        int mod_;
        std::vector<int> data_;
    };

    Poly PowMod(Poly a, int n, const Poly& m) {
        assert(a.Mod() == m.Mod());
        Poly res(a.Mod(), 1);
        while (n > 0) {
            if (n & 1) {
                res = res * a % m;
            }
            a = a * a % m;
            n >>= 1;
        }
        
        return res;
    }
};