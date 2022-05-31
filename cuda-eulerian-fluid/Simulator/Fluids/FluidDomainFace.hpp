#ifndef HF_SIMULATOR_FLUID_DOMAIN_FACE_HPP
#define HF_SIMULATOR_FLUID_DOMAIN_FACE_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct FluidDomainFace
    {
        None,
        Left,
        Right,
        Bottom,
        Top,
        Back,
        Front
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUID_DOMAIN_FACE_HPP */
