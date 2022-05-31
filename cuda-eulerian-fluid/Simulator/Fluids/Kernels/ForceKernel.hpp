#ifndef HF_SIMULATOR_FLUIDS_FORCE_KERNEL_HPP
#define HF_SIMULATOR_FLUIDS_FORCE_KERNEL_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/KernelConfig.hpp>
#include <Simulator/Compute/KernelThread.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint Order, typename LocationTag>
    struct ForceKernel
    {
        using Config = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;
        
        using Thread = Compute::KernelThread<Order>;
        using Domain = FluidDomain<Order>;
        using Coords = FloatN<Order>;
        using Index  = IntN<Order>;

        template <typename ForceConstAccessor, typename VelocityAccessor>
        static HF_HDINLINE void kernel(Thread             thread,
                                       Domain             dom,
                                       ForceConstAccessor forceField,
                                       VelocityAccessor   velocityField,
                                       float              timestep)
        {
            for (uint axis = 0; axis < Order; ++axis)
            {
                if (any(greaterThanEqual(thread.index, dom.getDimsOfFaceGrid(axis))))
                    continue;

                Index prevIdx = thread.index; --prevIdx[axis];
                const float force = 0.5f * (forceField[axis].getValue(thread.index) + forceField[axis].getValue(prevIdx));

                const float vel = velocityField[axis].getValue(thread.index);
                velocityField[axis].setValue(thread.index, vel + timestep * force);
            }
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUIDS_FORCE_KERNEL_HPP */
