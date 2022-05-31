#ifndef HF_SIMULATOR_FLUIDS_VELOCITY_ADVECTION_KERNEL_HPP
#define HF_SIMULATOR_FLUIDS_VELOCITY_ADVECTION_KERNEL_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/KernelThread.hpp>
#include <Simulator/Compute/KernelBlockDims.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>
#include <Simulator/Fluids/Kernels/Helpers.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint Order, typename LocationTag>
    struct VelocityAdvectionKernel
    {
        using Config = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;

        using Thread  = Compute::KernelThread<Order>;
        using Domain  = FluidDomain<Order>;
        using Coords  = FloatN<Order>;
        using Index   = UintN<Order>;
        using Helpers = Helpers<Order>;

        template <typename VelocitySampler,
                  typename VelocityAccessor>
        static HF_HDINLINE void kernel(Thread           thread,
                                       Domain           dom,
                                       VelocitySampler  velocityField,
                                       VelocityAccessor newVelocityField,
                                       float            timestep,
                                       float            dissipation)
        {
            for (uint axis = 0; axis < Order; ++axis)
            {
                if (any(greaterThanEqual(thread.index, dom.getDimsOfFaceGrid(axis))))
                    continue;

                // Sample position and velocity for the current face.

                Coords pos = dom.getFacePosition(thread.index, axis);
                Coords vel = Helpers::getStaggeredVectorAtPosition(dom, velocityField, pos);

                // Perform semilagrangian advection.

                float value = velocityField[axis].getValue(dom.getFaceCoords(pos - vel * timestep, axis));
                value -= value * timestep * dissipation;

                newVelocityField[axis].setValue(thread.index, value);
            }
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUIDS_VELOCITY_ADVECTION_KERNEL_HPP */
