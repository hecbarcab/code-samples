#ifndef HF_SIMULATOR_FLUIDS_VISCOSITY_KERNEL_HPP
#define HF_SIMULATOR_FLUIDS_VISCOSITY_KERNEL_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/KernelThread.hpp>
#include <Simulator/Compute/KernelBlockDims.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>
#include <Simulator/Fluids/Kernels/Helpers.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint Order, typename LocationTag>
    struct ViscosityKernel
    {
        using Config       = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;

        using Thread       = Compute::KernelThread<Order>;
        using Domain       = FluidDomain<Order>;
        using Coords       = FloatN<Order>;
        using Index        = UintN<Order>;
        using Helpers      = Helpers<Order>;

        template <typename VelocityConstAccessor,
                  typename VelocityAccessor>
        static HF_HDINLINE void kernel(Thread                thread,
                                       Domain                dom,
                                       VelocityConstAccessor velocityField,
                                       VelocityAccessor      newVelocityField,
                                       float                 timestepViscosityOverDensity)
        {
            const Coords oneOverDx = dom.getOneOverDx();
            const Coords oneOverDxSqr = oneOverDx * oneOverDx;

            for (uint axis = 0; axis < Order; ++axis)
            {
                const auto dims = dom.getDimsOfFaceGrid(axis);

                if (any(greaterThanEqual(thread.index, dims)))
                    continue;

                // Compute velocity laplacian.
                // Initialize it with the common component corresponding to the velocity
                // at the current index.

                float value = velocityField[axis].getValue(thread.index);
                float laplacian = -2.0f * value * compAdd(oneOverDxSqr);

                for (uint otherAxis = 0; otherAxis < Order; ++otherAxis)
                {
                    // Determine adjacent indices

                    Index prevIndex = thread.index; 
                    Index nextIndex = thread.index;
                    if (prevIndex[otherAxis] > 0)                   --prevIndex[otherAxis];
                    if (nextIndex[otherAxis] < dims[otherAxis] - 1) ++nextIndex[otherAxis];
                    
                    // Compute partial contribution to the laplacian.

                    const float prevValue = velocityField[axis].getValue(prevIndex);
                    const float nextValue = velocityField[axis].getValue(nextIndex);
                    laplacian += (prevValue + nextValue) * oneOverDxSqr[otherAxis];
                }

                // Integrate damping force.

                value += timestepViscosityOverDensity * laplacian;
                newVelocityField[axis].setValue(thread.index, value);
            }
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUIDS_VISCOSITY_KERNEL_HPP */
