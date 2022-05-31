#ifndef HF_SIMULATOR_FLUID_BOUNDS_HPP
#define HF_SIMULATOR_FLUID_BOUNDS_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct FluidBounds
    {
        None,
        Air,
        Solid
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUID_BOUNDS_HPP */
