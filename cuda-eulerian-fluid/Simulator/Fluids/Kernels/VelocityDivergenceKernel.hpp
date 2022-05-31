#ifndef HF_SIMULATOR_FLUIDS_VELOCITY_DIVERGENCE_HPP
#define HF_SIMULATOR_FLUIDS_VELOCITY_DIVERGENCE_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/KernelThread.hpp>
#include <Simulator/Compute/KernelBlockDims.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint Order, typename LocationTag>
    struct VelocityDivergenceKernel
    {
        using Config = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;

        using Thread = Compute::KernelThread<Order>;
        using Domain = FluidDomain<Order>;
        using Coords = FloatN<Order>;
        using Index  = IntN<Order>;

        template <typename VelocityConstAccessor,
                  typename DivergenceAccessor>
        static HF_HDINLINE void kernel(Thread                thread,
                                       Domain                dom,
                                       VelocityConstAccessor velocityField,
                                       DivergenceAccessor    divergenceField)
        {
            if (any(greaterThanEqual(thread.index, dom.getDims())))
                return;

            const Coords oneOverDx = dom.getOneOverDx();

            // Compute divergence.

            float divergence = 0;

            for (uint axis = 0; axis < Order; ++axis)
            {
                // Get coordinates of closest velocity field faces.

                Index prevIndex = thread.index;
                Index nextIndex = thread.index; ++nextIndex[axis];

                // Accumulate divergence.

                const float prevVelocity = velocityField[axis].getValue(prevIndex);
                const float nextVelocity = velocityField[axis].getValue(nextIndex);
                divergence += (nextVelocity - prevVelocity) * oneOverDx[axis];
            }

            divergenceField.setValue(thread.index, divergence);
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUIDS_VELOCITY_DIVERGENCE_HPP */
