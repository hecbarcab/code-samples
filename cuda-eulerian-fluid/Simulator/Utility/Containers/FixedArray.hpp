#ifndef HF_SIMULATOR_UTILITY_CONTAINERS_FIXED_ARRAY_HPP
#define HF_SIMULATOR_UTILITY_CONTAINERS_FIXED_ARRAY_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    /**
     * \brief Simple container class storing a fixed-size array. It's essentially an oversimplified
     *        version of STL's std::array -- but this one can be used in kernels :P
     * \tparam _Length Length (no. of elements) of the array.
     * \tparam _Value Value type of each element.
     */
    template <uint _Length, typename _Value>
    class FixedArray
    {
    public:
        static constexpr uint Length = _Length;
        using Value = _Value;

    public:
        FixedArray() = default;

    public:
        HF_HDINLINE const Value& operator[](size_t idx) const
        {
            return _array[idx];
        }

        HF_HDINLINE Value& operator[](size_t idx)
        {
            return _array[idx];
        }

    private:
        Value _array[Length];
    };
    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF)

#endif /* HF_SIMULATOR_UTILITY_CONTAINERS_FIXED_ARRAY_HPP */
