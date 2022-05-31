#ifndef HF_SIMULATOR_COMPUTE_BUFFER_FLAGS_HPP
#define HF_SIMULATOR_COMPUTE_BUFFER_FLAGS_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct BufferFlags
    {
        Default       = cudaHostAllocDefault,
        Portable      = cudaHostAllocPortable,
        Mapped        = cudaHostAllocMapped,
        WriteCombined = cudaHostAllocWriteCombined
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_BUFFER_FLAGS_HPP */

