#ifndef HF_SIMULATOR_FLUID_DOMAIN_BOUNDS_VELOCITY_HPP
#define HF_SIMULATOR_FLUID_DOMAIN_BOUNDS_VELOCITY_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Geometry/Primitives/AABB.hpp>
#include <Simulator/Fluids/FluidBounds.hpp>
#include <Simulator/Fluids/FluidDomainFace.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    class FluidDomainBoundsVelocity
    {
    public:
        static constexpr uint Order = _Order;

    public:
        FluidDomainBoundsVelocity()
        {
            for (uint i = 0; i < 2 * Order; ++i)
            {
                _velocities[i] = 0.0f;
            }
        }

    public:
        HF_HDINLINE float getFaceVelocity(FluidDomainFace face) const
        {
            return (face != FluidDomainFace::None) 
                ? _velocities[getIndexOfFace(face)] 
                : 0.0f;
        }

        HF_HINLINE void setFaceVelocity(FluidDomainFace face, float velocity)
        {
            if (face != FluidDomainFace::None) 
                _velocities[getIndexOfFace(face)] = velocity;
        }

    private:
        HF_HDINLINE uint getIndexOfFace(FluidDomainFace face) const
        {
            return static_cast<uint>(face) - 1;
        }

    private:
        float _velocities[2 * Order];
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using FluidDomainBoundsVelocity1 = FluidDomainBoundsVelocity<1>;
    using FluidDomainBoundsVelocity2 = FluidDomainBoundsVelocity<2>;
    using FluidDomainBoundsVelocity3 = FluidDomainBoundsVelocity<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUID_DOMAIN_BOUNDS_VELOCITY_HPP */
