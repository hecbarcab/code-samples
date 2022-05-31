#ifndef HF_SIMULATOR_COMPUTE_SURFACE_BOUNDARY_MODE_HPP
#define HF_SIMULATOR_COMPUTE_SURFACE_BOUNDARY_MODE_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct SurfaceBoundaryMode
    {
        Zero  = cudaBoundaryModeZero,
        Clamp = cudaBoundaryModeClamp,
        Trap  = cudaBoundaryModeTrap
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_SURFACE_BOUNDARY_MODE_HPP */
