#ifndef HF_SIMULATOR_COMPUTE_COPY_REGION_HPP
#define HF_SIMULATOR_COMPUTE_COPY_REGION_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    struct CopyRegion
    {
    public:
        static constexpr uint Order = _Order;
        using Offset = IntN<Order>;
        using Size   = IntN<Order>;

    public:
        CopyRegion(const Size& size)
            : srcOffset(0)
            , dstOffset(0)
            , size(size)
        {
        }

        CopyRegion(const Offset& offset, const Size& size)
            : srcOffset(offset)
            , dstOffset(offset)
            , size(size)
        {
        }

        CopyRegion(const Offset& srcOffset, const Offset& dstOffset,
                   const Size& size)
            : srcOffset(srcOffset)
            , dstOffset(dstOffset)
            , size(size)
        {
        }

    public:
        Offset srcOffset;
        Offset dstOffset;
        Size   size;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using CopyRegion1 = CopyRegion<1>;
    using CopyRegion2 = CopyRegion<2>;
    using CopyRegion3 = CopyRegion<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_COPY_REGION_HPP */
