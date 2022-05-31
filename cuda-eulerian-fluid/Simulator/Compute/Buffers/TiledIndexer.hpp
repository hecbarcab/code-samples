#ifndef HF_SIMULATOR_COMPUTE_BUFFERS_TILED_INDEXER_HPP
#define HF_SIMULATOR_COMPUTE_BUFFERS_TILED_INDEXER_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order, int _TileDims>
    struct TiledIndexer
    {
    public:
        using Dims                    = IntN<_Order>;
        using Index                   = IntN<_Order>;
        static constexpr uint Order   = _Order;
        static constexpr int TileDims = _TileDims;

    public:
        TiledIndexer() = default;

    public:
        /**
         * \brief 
         * \param size 
         * \return 
         */
        HF_HINLINE uint setup(const Dims& size)
        {
            const Dims tileCount = (size + Dims(TileDims - 1)) / TileDims;
            const Dims totalSize = TileDims * tileCount;

            _elemStride = exclusiveCumProd(Dims(TileDims));
            _tileStride = exclusiveCumProd(tileCount) * compMul(Dims(TileDims));

            return compMul(totalSize);
        }

        /**
         * \brief 
         * \param idx 
         * \return 
         */
        HF_HDINLINE int computeOffset(const Index &idx) const
        {
            const Index tileIdx = idx / int(TileDims);
            const Index elemIdx = idx % int(TileDims);
            return compAdd(tileIdx * _tileStride) + compAdd(elemIdx * _elemStride);
        }

    private:
        Dims _elemStride;
        Dims _tileStride;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_BUFFERS_TILED_INDEXER_HPP */
