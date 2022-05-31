#ifndef HF_SIMULATOR_FLUIDS_ADVECTION_HPP
#define HF_SIMULATOR_FLUIDS_ADVECTION_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/KernelConfig.hpp>
#include <Simulator/Compute/KernelThread.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>
#include <Simulator/Fluids/Kernels/Helpers.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint Order, typename LocationTag>
    struct AdvectionKernel
    {
        using Config = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;

        using Thread  = Compute::KernelThread<Order>;
        using Domain  = FluidDomain<Order>;
        using Coords  = FloatN<Order>;
        using Index   = IntN<Order>;
        using Helpers = Helpers<Order>;

        template <typename ValueSampler,
                  typename VelocityAccessor,
                  typename ValueAccessor>
        static HF_HDINLINE void kernel(Thread           thread,
                                       Domain           dom,
                                       ValueSampler     valueField,
                                       VelocityAccessor velocityField,
                                       ValueAccessor    newValueField,
                                       float            timestep)
        {
            if (any(greaterThanEqual(thread.index, dom.getDims())))
                return;

            // Sample position and velocity for the current cell.

            Coords pos = dom.getCellPosition(thread.index);
            Coords vel = Helpers::getStaggeredVectorAtCell(velocityField, thread.index);

            // Perform semilagrangian advection.

            auto value = valueField.getValue(dom.getCellCoords(pos - vel * timestep));
            newValueField.setValue(thread.index, value);
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUIDS_ADVECTION_HPP */
