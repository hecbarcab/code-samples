#ifndef HF_SIMULATOR_UTILITY_BITPATTERN_HPP
#define HF_SIMULATOR_UTILITY_BITPATTERN_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename Type, uint HiPeriod, uint LoPeriod>
    struct BitPattern
    {
        static_assert(std::is_integral<Type>(), "Bit pattern might only be computed for integral types.");
        static_assert(HiPeriod > 0 && LoPeriod > 0, "Both the high and low bit periods should be greater than zero.");

        static constexpr uint Count = 8ull * sizeof(Type);

        template <uint B> struct Bit  { static constexpr Type Value = (B % (HiPeriod + LoPeriod)) < LoPeriod; };
        template <uint B> struct Seq  { static constexpr Type Value = Bit<B>::Value | Seq<B + 1>::Value << 1; };
        template <> struct Seq<Count> { static constexpr Type Value = 0; };

        static constexpr Type Value = Seq<0>::Value;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF)

#endif /* HF_SIMULATOR_UTILITY_BITPATTERN_HPP */
