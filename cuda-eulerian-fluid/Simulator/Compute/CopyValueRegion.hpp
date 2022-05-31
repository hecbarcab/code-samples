#ifndef HF_SIMULATOR_COMPUTE_COPY_VALUE_REGION_HPP
#define HF_SIMULATOR_COMPUTE_COPY_VALUE_REGION_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    struct CopyValueRegion
    {
    public:
        static constexpr uint Order = _Order;
        using Offset = IntN<Order>;
        using Size   = IntN<Order>;

    public:
        CopyValueRegion(const Size& size)
            : offset(0)
            , size(size)
        {
        }

        CopyValueRegion(const Offset& offset, const Size& size)
            : offset(offset)
            , size(size)
        {
        }

    public:
        Offset offset;
        Size   size;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using CopyValueRegion1 = CopyValueRegion<1>;
    using CopyValueRegion2 = CopyValueRegion<2>;
    using CopyValueRegion3 = CopyValueRegion<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_COPY_VALUE_REGION_HPP */
