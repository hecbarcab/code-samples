#ifndef HF_SIMULATOR_UTILITY_BITSIZEOF_HPP
#define HF_SIMULATOR_UTILITY_BITSIZEOF_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename T> 
    HF_HDINLINE constexpr size_t BitSizeOf()
    {
        return 8 * sizeof(T);
    }

    template <typename T> 
    HF_HDINLINE constexpr size_t BitSizeOf(const T& x)
    {
        return 8 * sizeof(x);
    }

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF)

#endif /* HF_SIMULATOR_UTILITY_BITSIZEOF_HPP */
