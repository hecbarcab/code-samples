#ifndef HF_SIMULATOR_COMPUTE_BUFFERS_LINEAR_INDEXER_HPP
#define HF_SIMULATOR_COMPUTE_BUFFERS_LINEAR_INDEXER_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    struct LinearIndexer
    {
    public:
        using Dims                  = IntN<_Order>;
        using Index                 = IntN<_Order>;
        static constexpr uint Order = _Order;

    public:
        LinearIndexer() = default;

    public:
        /**
         * \brief 
         * \param size 
         * \return 
         */
        HF_HINLINE uint setup(const Dims& size)
        {
            _stride = exclusiveCumProd(size);
            return compMul(size);
        }

        /**
         * \brief 
         * \param idx 
         * \return 
         */
        HF_HDINLINE int computeOffset(const Index &idx) const
        {
            return compAdd(idx * _stride);
        }

    private:
        Index _stride;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_BUFFERS_LINEAR_INDEXER_HPP */
