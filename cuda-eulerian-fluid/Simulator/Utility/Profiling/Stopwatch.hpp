#ifndef HF_SIMULATOR_UTILITY_PROFILING_STOPWATCH_HPP
#define HF_SIMULATOR_UTILITY_PROFILING_STOPWATCH_HPP

#include <chrono>
#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename Clock, typename Ratio, typename Scalar>
    class BasicStopwatch
    {
    public:
        using TimePoint = std::chrono::time_point<Clock>;
        using Duration  = std::chrono::duration<Scalar, Ratio>;

    public:
        BasicStopwatch()
            : _running(false)
        {
        }

    public:
        bool isRunning() const
        {
            return _running;
        }

        Scalar getElapsed() const
        {
            const TimePoint currentTime = Clock::now();
            const Scalar duration = std::chrono::duration_cast<Duration>(currentTime - _lastTime).count();
            _lastTime = currentTime;

            return duration;
        }

        Scalar getTotal() const
        {
            const TimePoint currentTime = Clock::now();
            const Scalar duration = std::chrono::duration_cast<Duration>(currentTime - _startTime).count();
            return duration;
        }

    public:
        void start()
        {
            _running = true;
            _startTime = _lastTime = Clock::now();
        }

        Scalar end()
        {
            Scalar duration = getTotal();
            _running = false;
            return duration;
        }

    private:
        bool              _running;
        TimePoint         _startTime;
        mutable TimePoint _lastTime;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using Stopwatch       = BasicStopwatch<std::chrono::steady_clock, std::milli, double>;
    using SystemStopwatch = BasicStopwatch<std::chrono::system_clock, std::milli, double>;
    using HiResStopwatch  = BasicStopwatch<std::chrono::high_resolution_clock, std::milli, double>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF)

#endif /* HF_SIMULATOR_UTILITY_PROFILING_STOPWATCH_HPP */
