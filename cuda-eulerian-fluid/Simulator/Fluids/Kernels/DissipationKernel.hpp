#ifndef HF_SIMULATOR_FLUIDS_DISSIPATION_KERNEL_HPP
#define HF_SIMULATOR_FLUIDS_DISSIPATION_KERNEL_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/KernelConfig.hpp>
#include <Simulator/Compute/KernelThread.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint Order, typename LocationTag>
    struct DissipationKernel
    {
        using Config = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;
        
        using Thread = Compute::KernelThread<Order>;
        using Domain = FluidDomain<Order>;
        using Coords = FloatN<Order>;
        using Index  = IntN<Order>;

        template <typename ValueAccessor,
                  typename Dissipation>
        static HF_HDINLINE void kernel(Thread        thread,
                                       Domain        dom,
                                       ValueAccessor valueField,
                                       Dissipation   dissipationByTimestep)
        {
            if (any(greaterThanEqual(thread.index, dom.getDims())))
                return;

            auto value = valueField.getValue(thread.index);
            value -= value * dissipationByTimestep;
            valueField.setValue(thread.index, value);
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUIDS_DISSIPATION_KERNEL_HPP */
