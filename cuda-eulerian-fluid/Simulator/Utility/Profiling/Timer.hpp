#ifndef HF_SIMULATOR_UTILITY_PROFILING_TIMER_HPP
#define HF_SIMULATOR_UTILITY_PROFILING_TIMER_HPP

#include <iostream>
#include <string>
#include <chrono>
#include <list>
#include <Simulator/Utility/Profiling/TimeRatio.hpp>
#include <Simulator/Utility/Macros.hpp>

HF_BEGIN_NAMESPACE(HF)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename Clock, typename Ratio, typename Scalar>
    class BasicTimer
    {
    public:
        using TimePoint = std::chrono::time_point<Clock>;
        using Duration  = std::chrono::duration<Scalar, Ratio>;

    private:
        struct Entry
        {
            std::string message;
            bool        print;
            TimePoint   startTime;

            Entry(const std::string& message, const bool print)
                : message(message.empty() ? "" : message + ": ")
                , print(print)
                , startTime(Clock::now())
            {
            }
        };

    public:
        void begin(const std::string& message = "", const bool print = false)
        {
            _entries.emplace_back(message, print);
        }

        Scalar end()
        {
            const Entry& entry = _entries.back();

            Scalar duration = std::chrono::duration_cast<Duration>(Clock::now() - entry).count();
            
            if (entry._print)
                std::cout << entry._message << duration << " " << TimeRatio<Ratio>::name << std::endl;

            _entries.pop_back();
            return duration;
        }

    private:
        std::list<Entry> _entries;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using Timer       = BasicTimer<std::chrono::steady_clock, std::milli, double>;
    using SystemTimer = BasicTimer<std::chrono::system_clock, std::milli, double>;
    using HiResTimer  = BasicTimer<std::chrono::high_resolution_clock, std::milli, double>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF)

#endif /* HF_SIMULATOR_UTILITY_PROFILING_TIMER_HPP */
