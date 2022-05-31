#ifndef HF_SIMULATOR_COMPUTE_TEXTURE_FILTER_MODE_HPP
#define HF_SIMULATOR_COMPUTE_TEXTURE_FILTER_MODE_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct TextureFilterMode
    {
        PointFilter  = cudaFilterModePoint,
        LinearFilter = cudaFilterModeLinear
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_TEXTURE_FILTER_MODE_HPP */

