#ifndef HF_SIMULATOR_COMPUTE_BUFFER_COORD_MODE_HPP
#define HF_SIMULATOR_COMPUTE_BUFFER_COORD_MODE_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct BufferCoordMode
    {
        BufferSpace     = 0,
        NormalizedSpace = 1
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_BUFFER_COORD_MODE_HPP */

