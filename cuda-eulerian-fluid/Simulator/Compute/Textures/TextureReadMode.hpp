#ifndef HF_SIMULATOR_COMPUTE_TEXTURE_READ_MODE_HPP
#define HF_SIMULATOR_COMPUTE_TEXTURE_READ_MODE_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct TextureReadMode
    {
        ElementType     = cudaReadModeElementType,
        NormalizedFloat = cudaReadModeNormalizedFloat
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_TEXTURE_READ_MODE_HPP */

