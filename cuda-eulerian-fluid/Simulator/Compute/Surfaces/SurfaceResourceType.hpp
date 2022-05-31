#ifndef HF_SIMULATOR_COMPUTE_SURFACE_RESOURCE_TYPE_HPP
#define HF_SIMULATOR_COMPUTE_SURFACE_RESOURCE_TYPE_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct SurfaceResourceType
    {
        Unknown = -1,
        Array   = cudaResourceTypeArray,
        Mipmap  = cudaResourceTypeMipmappedArray,
        Linear  = cudaResourceTypeLinear,
        Pitch2D = cudaResourceTypePitch2D
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_SURFACE_RESOURCE_TYPE_HPP */

