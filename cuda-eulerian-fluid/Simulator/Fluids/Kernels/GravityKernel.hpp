#ifndef HF_SIMULATOR_FLUIDS_GRAVITY_KERNEL_HPP
#define HF_SIMULATOR_FLUIDS_GRAVITY_KERNEL_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/KernelConfig.hpp>
#include <Simulator/Compute/KernelThread.hpp>
#include <Simulator/Compute/KernelBlockDims.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint Order, typename LocationTag>
    struct GravityKernel
    {
        using Config = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;
        
        using Thread = Compute::KernelThread<Order>;
        using Domain = FluidDomain<Order>;
        using Coords = FloatN<Order>;
        using Index  = IntN<Order>;

        template <typename VelocityAccessor>
        static HF_HDINLINE void kernel(Thread           thread,
                                       Domain           dom,
                                       VelocityAccessor velocityField,
                                       Coords           gravity,
                                       float            timestep)
        {
            for (uint axis = 0; axis < Order; ++axis)
            {
                if (any(greaterThanEqual(thread.index, dom.getDimsOfFaceGrid(axis))))
                    continue;

                float vel = velocityField[axis].getValue(thread.index);
                vel += timestep * gravity[axis];

                velocityField[axis].setValue(thread.index, vel);
            }
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUIDS_GRAVITY_KERNEL_HPP */
