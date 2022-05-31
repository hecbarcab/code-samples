#ifndef HF_SIMULATOR_COMPUTE_ARRAY_FLAGS_HPP
#define HF_SIMULATOR_COMPUTE_ARRAY_FLAGS_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct ArrayFlags
    {
        Default          = cudaArrayDefault,
        SurfaceLoadStore = cudaArraySurfaceLoadStore,
        TextureGather    = cudaArrayTextureGather
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_ARRAY_FLAGS_HPP */

