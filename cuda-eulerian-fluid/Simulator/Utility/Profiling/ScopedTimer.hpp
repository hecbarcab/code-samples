#ifndef HF_SIMULATOR_UTILITY_PROFILING_SCOPEDTIMER_HPP
#define HF_SIMULATOR_UTILITY_PROFILING_SCOPEDTIMER_HPP

#include <iostream>
#include <string>
#include <chrono>
#include <Simulator/Simulator.hpp>
#include <Simulator/Utility/Profiling/TimeRatio.hpp>

HF_BEGIN_NAMESPACE(HF)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename Clock, typename Ratio, typename Scalar>
    class BasicScopedTimer
    {
    public:
        using TimePoint = std::chrono::time_point<Clock>;
        using Duration  = std::chrono::duration<Scalar, Ratio>;

    public:
        BasicScopedTimer(const bool print = true, const std::string& message = "")
            : _results(nullptr)
            , _print(print)
            , _message(message.empty() ? "" : message + ": ")
            , _startTime(Clock::now())
        {
        }

        BasicScopedTimer(double& results, const bool print = true, const std::string& message = "")
            : _results(&results)
            , _print(print)
            , _message(message.empty() ? "" : message + ": ")
            , _startTime(Clock::now())
        {
        }

        ~BasicScopedTimer()
        {
            auto duration = std::chrono::duration_cast<Duration>(Clock::now() - _startTime).count();
            
            if (_print)
                std::cout << _message << duration << " " << TimeRatio<Ratio>::name << std::endl;

            if (_results != nullptr)
                *_results = duration;
        }

    public:
        BasicScopedTimer(const BasicScopedTimer&) = delete;
        BasicScopedTimer(const BasicScopedTimer&&) = delete;
        BasicScopedTimer& operator=(const BasicScopedTimer&) = delete;
        BasicScopedTimer&& operator=(const BasicScopedTimer&&) = delete;

    private:
        double*                         _results;
        bool                            _print;
        std::string                     _message;
        std::chrono::time_point<Clock> _startTime;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using ScopedTimer       = BasicScopedTimer<std::chrono::steady_clock, std::ratio<1>, double>;
    using SystemScopedTimer = BasicScopedTimer<std::chrono::system_clock, std::ratio<1>, double>;
    using HiResScopedTimer  = BasicScopedTimer<std::chrono::high_resolution_clock, std::ratio<1>, double>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF)

#endif /* HF_SIMULATOR_UTILITY_PROFILING_SCOPEDTIMER_HPP */
