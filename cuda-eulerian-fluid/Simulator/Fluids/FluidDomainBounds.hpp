#ifndef HF_SIMULATOR_FLUID_DOMAIN_BOUNDS_HPP
#define HF_SIMULATOR_FLUID_DOMAIN_BOUNDS_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Geometry/Primitives/AABB.hpp>
#include <Simulator/Fluids/FluidBounds.hpp>
#include <Simulator/Fluids/FluidDomainFace.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    class FluidDomainBounds
    {
    public:
        static constexpr uint Order = _Order;

    public:
        FluidDomainBounds()
        {
            for (uint i = 0; i < 2 * Order; ++i)
            {
                _boundaries[i] = FluidBounds::Solid;
            }
        }

    public:
        HF_HDINLINE FluidBounds getFaceBoundary(FluidDomainFace face) const
        {
            return (face != FluidDomainFace::None) 
                ? _boundaries[getIndexOfFace(face)] 
                : FluidBounds::None;
        }

        HF_HINLINE void setFaceBoundary(FluidDomainFace face, FluidBounds boundary)
        {
            if (face != FluidDomainFace::None) 
                _boundaries[getIndexOfFace(face)] = boundary;
        }

    private:
        HF_HDINLINE uint getIndexOfFace(FluidDomainFace face) const
        {
            return static_cast<uint>(face) - 1;
        }

    private:
        FluidBounds _boundaries[2 * Order];
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using FluidDomainBounds1 = FluidDomainBounds<1>;
    using FluidDomainBounds2 = FluidDomainBounds<2>;
    using FluidDomainBounds3 = FluidDomainBounds<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUID_DOMAIN_BOUNDS_HPP */
