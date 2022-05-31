#ifndef HF_SIMULATOR_FLUIDS_PRESSURE_JACOBI_KERNEL_HPP
#define HF_SIMULATOR_FLUIDS_PRESSURE_JACOBI_KERNEL_HPP

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
    struct PressureJacobiKernel
    {
        using Config = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;
        
        using Thread       = Compute::KernelThread<Order>;
        using Domain       = FluidDomain<Order>;
        using DomainBounds = FluidDomainBounds<Order>;
        using Coords       = FloatN<Order>;
        using Index        = IntN<Order>;
        using Helpers      = Helpers<Order>;

        template <typename PressureConstAccessor,
                  typename DivergenceConstAccessor,
                  typename BoundaryConstAccessor,
                  typename PressureAccessor>
        static HF_HDINLINE void kernel(Thread                  thread,
                                       Domain                  dom,
                                       DomainBounds            domBounds,
                                       PressureConstAccessor   pressureField,
                                       DivergenceConstAccessor divergenceField,
                                       BoundaryConstAccessor   boundaryField,
                                       PressureAccessor        newPressureField,
                                       float                   restDensityOverTimestep)
        {
            if (any(greaterThanEqual(thread.index, dom.getDims())))
                return;

            // If the current cell is a boundary, skip.

            if (Helpers::getBoundaryAtCell(dom, domBounds, boundaryField, thread.index) != FluidBounds::None)
            {
                newPressureField.setValue(thread.index, 0.0f);
            }
            else
            {
                // Fetch position at center cell.

                const Coords oneOverDx = dom.getOneOverDx();
                const Coords oneOverDxSqr = oneOverDx * oneOverDx;

                // Retrieve divergence and pressure at the cell

                const float divergence = divergenceField.getValue(thread.index);
                const float pressure = pressureField.getValue(thread.index);

                // Compute pressure laplacian taking into account boundary cells.

                float pressureLaplacian = 0.0f;

                for (uint axis = 0; axis < Order; ++axis)
                {
                    // Lookup neighbor cells and compute pressure laplacian. Take into account boundary conditions.

                    Index prevIndex = thread.index; --prevIndex[axis];
                    Index nextIndex = thread.index; ++nextIndex[axis];

                    const FluidBounds prevBoundary = Helpers::getBoundaryAtCell(dom, domBounds, boundaryField, prevIndex);
                    const FluidBounds nextBoundary = Helpers::getBoundaryAtCell(dom, domBounds, boundaryField, nextIndex);

                    const float prevPressure = prevBoundary == FluidBounds::Air ? 0.0f
                        : prevBoundary == FluidBounds::Solid ? pressure
                        : pressureField.getValue(prevIndex);

                    const float nextPressure = nextBoundary == FluidBounds::Air ? 0.0f
                        : nextBoundary == FluidBounds::Solid ? pressure
                        : pressureField.getValue(nextIndex);

                    pressureLaplacian += oneOverDxSqr[axis] * (nextPressure + prevPressure - 2.0f * pressure);
                }

                // Compute pressure delta.
                // Δp = D⁻¹((ρ/Δt)∇·u - ∇²·p)

                //const float oneOverD = -1.0f / (2.0f * compAdd(oneOverDxSqr));
                //const float deltaPressure = oneOverD * (restDensityOverTimestep * divergence - pressureLaplacian);
                const float deltaPressure = -0.5f * (restDensityOverTimestep * divergence - pressureLaplacian) / compAdd(oneOverDxSqr);

                // Store new pressure.

                const float newPressure = pressure + deltaPressure;
                newPressureField.setValue(thread.index, newPressure);
            }
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUIDS_PRESSURE_JACOBI_KERNEL_HPP */
