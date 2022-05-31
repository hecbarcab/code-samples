#ifndef HF_SIMULATOR_COMPUTE_BUFFERS_MORTON_INDEXER_HPP
#define HF_SIMULATOR_COMPUTE_BUFFERS_MORTON_INDEXER_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Utility/Bitwise/Morton.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    struct MortonIndexer
    {
    public:
        using Dims                  = IntN<_Order>;
        using Index                 = IntN<_Order>;
        static constexpr uint Order = _Order;

    public:
        MortonIndexer() = default;

    public:
        /**
         * \brief 
         * \param dims 
         * \return 
         */
        HF_HINLINE uint setup(const Dims& dims)
        {
            return compMul(dims);
        }

        /**
         * \brief 
         * \param idx 
         * \return 
         */
        HF_HDINLINE int computeOffset(const Index &idx) const
        {
            int offset = 0;

            for (uint i = 0; i < Order; ++i)
                offset |= Morton<int, Order>::Encode(idx[i]) << i;

            return offset;
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_BUFFERS_MORTON_INDEXER_HPP */
