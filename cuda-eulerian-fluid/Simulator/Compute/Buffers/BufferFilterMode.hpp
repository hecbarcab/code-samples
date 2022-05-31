#ifndef HF_SIMULATOR_COMPUTE_BUFFER_FILTER_MODE_HPP
#define HF_SIMULATOR_COMPUTE_BUFFER_FILTER_MODE_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct BufferFilterMode
    {
        PointFilter,
        LinearFilter
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_BUFFER_FILTER_MODE_HPP */

