#ifndef HF_SIMULATOR_FLUIDS_PRESSURE_JACOBI_PROJECTION_KERNEL_HPP
#define HF_SIMULATOR_FLUIDS_PRESSURE_JACOBI_PROJECTION_KERNEL_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/KernelThread.hpp>
#include <Simulator/Compute/KernelBlockDims.hpp>
#include <Simulator/Fluids/FluidBounds.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>
#include <Simulator/Fluids/FluidDomainBounds.hpp>
#include <Simulator/Fluids/Kernels/Helpers.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint Order, typename LocationTag>
    struct PressureJacobiProjectionKernel
    {
        using Config = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;

        using Thread       = Compute::KernelThread<Order>;
        using Domain       = FluidDomain<Order>;
        using DomainBounds = FluidDomainBounds<Order>;
        using Coords       = FloatN<Order>;
        using Index        = IntN<Order>;
        using Helpers      = Helpers<Order>;

        template <typename PressureConstAccessor,
                  typename BoundaryConstAccessor,
                  typename VelocityAccessor,
                  typename PressureGradNormAccessor>
        static HF_HDINLINE void kernel(Thread                   thread,
                                       Domain                   dom,
                                       DomainBounds             domBounds,
                                       PressureConstAccessor    pressureField,
                                       BoundaryConstAccessor    boundaryField,
                                       VelocityAccessor         velocityField,
                                       PressureGradNormAccessor pressureGradNormField,
                                       float                    timestepOverRestDensity)
        {
            const Coords oneOverDx = dom.getOneOverDx();

            float pressureGradNorm = 0.0f;

            for (uint axis = 0; axis < Order; ++axis)
            {
                if (any(greaterThanEqual(thread.index, dom.getDimsOfFaceGrid(axis))))
                    continue;

                // Fetch velocity at current index

                float vel = velocityField[axis].getValue(thread.index);

                // Lookup neighbor cells and compute pressure gradient. Take into account boundary conditions.

                Index prevIndex = thread.index; --prevIndex[axis];
                Index nextIndex = thread.index;

                const FluidBounds prevBoundary = Helpers::getBoundaryAtCell(dom, domBounds, boundaryField, prevIndex);
                const FluidBounds nextBoundary = Helpers::getBoundaryAtCell(dom, domBounds, boundaryField, nextIndex);

                // Note: if both cells are boundaries, pressure gradient is zero, and therefore velocity does not change.

                if (prevBoundary == FluidBounds::None || nextBoundary == FluidBounds::None)
                {
                    const float prevPressure = prevBoundary == FluidBounds::Air ? 0.0f
                                             : prevBoundary == FluidBounds::Solid ? pressureField.getValue(nextIndex)
                                             : pressureField.getValue(prevIndex);

                    const float nextPressure = nextBoundary == FluidBounds::Air ? 0.0f
                                             : nextBoundary == FluidBounds::Solid ? pressureField.getValue(prevIndex)
                                             : pressureField.getValue(nextIndex);

                    vel -= timestepOverRestDensity * oneOverDx[axis] * (nextPressure - prevPressure);
                    pressureGradNorm += (nextPressure - prevPressure) * (nextPressure - prevPressure);
                }

                // Store new velocities

                velocityField[axis].setValue(thread.index, vel);
                pressureGradNormField.setValue(thread.index, sqrt(pressureGradNorm));
            }
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUIDS_PRESSURE_JACOBI_PROJECTION_KERNEL_HPP */
