#ifndef HF_SIMULATOR_UTILITY_PROFILING_TIMER_RATIO_HPP
#define HF_SIMULATOR_UTILITY_PROFILING_TIMER_RATIO_HPP

#include <string>
#include <chrono>
#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename Ratio> struct TimeRatio;
    template <> struct TimeRatio<std::ratio<1, 1>> { static constexpr std::string::const_pointer name = "s";  };
    template <> struct TimeRatio<std::deci>        { static constexpr std::string::const_pointer name = "ds"; };
    template <> struct TimeRatio<std::centi>       { static constexpr std::string::const_pointer name = "cs"; };
    template <> struct TimeRatio<std::milli>       { static constexpr std::string::const_pointer name = "ms"; };
    template <> struct TimeRatio<std::micro>       { static constexpr std::string::const_pointer name = "us"; };
    template <> struct TimeRatio<std::nano>        { static constexpr std::string::const_pointer name = "ns"; };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF)

#endif /* HF_SIMULATOR_UTILITY_PROFILING_TIMER_RATIO_HPP */
