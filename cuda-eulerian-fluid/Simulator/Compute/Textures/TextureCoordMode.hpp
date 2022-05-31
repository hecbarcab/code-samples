#ifndef HF_SIMULATOR_COMPUTE_TEXTURE_COORD_MODE_HPP
#define HF_SIMULATOR_COMPUTE_TEXTURE_COORD_MODE_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct TextureCoordMode
    {
        TextureSpace    = 0,
        NormalizedSpace = 1
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_TEXTURE_COORD_MODE_HPP */

