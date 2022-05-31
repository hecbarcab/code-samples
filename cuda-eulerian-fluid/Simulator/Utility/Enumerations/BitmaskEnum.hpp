#ifndef HF_SIMULATOR_BITMASKENUM_HPP
#define HF_SIMULATOR_BITMASKENUM_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────────

    // https://www.justsoftwaresolutions.co.uk/cplusplus/using-enum-classes-as-bitfields.html

    template<typename E>
    struct BitmaskEnumTraits
    {
        static const bool Enabled = false;
    };

    template<typename E>
    typename std::enable_if<BitmaskEnumTraits<E>::Enabled, E>::type operator|(E lhs, E rhs)
    {
        typedef typename std::underlying_type<E>::type U;
        return static_cast<E>(static_cast<U>(lhs) | static_cast<U>(rhs));
    }

    template<typename E>
    typename std::enable_if<BitmaskEnumTraits<E>::Enabled, E>::type operator&(E lhs, E rhs)
    {
        typedef typename std::underlying_type<E>::type U;
        return static_cast<E>(static_cast<U>(lhs) & static_cast<U>(rhs));
    }

    template<typename E>
    typename std::enable_if<BitmaskEnumTraits<E>::Enabled, E>::type operator^(E lhs, E rhs)
    {
        typedef typename std::underlying_type<E>::type U;
        return static_cast<E>(static_cast<U>(lhs) ^ static_cast<U>(rhs));
    }

    template<typename E>
    typename std::enable_if<BitmaskEnumTraits<E>::Enabled, E>::type operator~(E lhs)
    {
        typedef typename std::underlying_type<E>::type U;
        return static_cast<E>(~static_cast<U>(lhs));
    }

    template<typename E>
    typename std::enable_if<BitmaskEnumTraits<E>::Enabled, E&>::type operator|=(E& lhs, E rhs)
    {
        typedef typename std::underlying_type<E>::type U;
        lhs = static_cast<E>(static_cast<U>(lhs) | static_cast<U>(rhs));
        return lhs;
    }

    template<typename E>
    typename std::enable_if<BitmaskEnumTraits<E>::Enabled, E&>::type operator&=(E& lhs, E rhs)
    {
        typedef typename std::underlying_type<E>::type U;
        lhs = static_cast<E>(static_cast<U>(lhs) & static_cast<U>(rhs));
        return lhs;
    }

    template<typename E>
    typename std::enable_if<BitmaskEnumTraits<E>::Enabled, E&>::type operator^=(E& lhs, E rhs) {
        typedef typename std::underlying_type<E>::type U;
        lhs = static_cast<E>(static_cast<U>(lhs) ^ static_cast<U>(rhs));
        return lhs;
    }

    //─────────────────────────────────────────────────────────────────────────────────────────────────

    #define HF_BITMASK_ENUM(E) template<> struct BitmaskEnumTraits<E> { static constexpr bool Enabled = true; }

    //─────────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF)

#endif /* HF_SIMULATOR_BITMASKENUM_HPP */
