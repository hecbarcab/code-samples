#ifndef HF_SIMULATOR_FLUID_PROPERTY_HPP
#define HF_SIMULATOR_FLUID_PROPERTY_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct FluidProperty
    {
        Ink,
        Velocity,
        VelocityDivergence,
        Vorticity,
        VorticityNorm,
        Confinement,
        Pressure,
        PressureGradNorm,
        Boundary,
        BoundaryDistance,
        BoundaryVelocity,
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUID_PROPERTY_HPP */
