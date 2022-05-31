#ifndef HF_SIMULATION_FLUID_PARAMS_HPP
#define HF_SIMULATION_FLUID_PARAMS_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Geometry/Primitives/AABB.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    class FluidParams
    {
    public:
        static constexpr uint Order = _Order;
        using Dims   = IntN<_Order>;
        using Coords = FloatN<_Order>;
        using AABB   = Geometry::AABB<_Order>;

    public:
        AABB   region;
        Dims   dims;
        float  timestep;
        Coords gravity;
        float  density;
        float  viscosity;
        float  confinement;
        uint   jacobiSteps;
        Float4 inkDissipation;
        float  velocityDissipation;
        float  pressureDissipation;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using FluidParams1 = FluidParams<1>;
    using FluidParams2 = FluidParams<2>;
    using FluidParams3 = FluidParams<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATION_FLUID_PARAMS_HPP */
