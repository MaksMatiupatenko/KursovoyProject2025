#pragma once
#include <chrono>

namespace prime {
    class Clock {
    public:
        Clock() {
            start_ = GetTicks();
        }

        float Time() const {
            return (GetTicks() - start_) / tickRatio_;
        }

        float Restart() {
            float res = (GetTicks() - start_) / tickRatio_;
            start_ = GetTicks();
            return res;
        }

    private:
        static unsigned long long GetTicks() {
            return std::chrono::high_resolution_clock::now().time_since_epoch().count();
        }

        static constexpr float tickRatio_ = 1000000000;
        unsigned long long start_;
    };
};
