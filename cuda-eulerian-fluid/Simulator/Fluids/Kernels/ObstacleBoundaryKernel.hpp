#ifndef HF_SIMULATOR_FLUIDS_OBSTACLE_BOUNDARY_KERNEL_HPP
#define HF_SIMULATOR_FLUIDS_OBSTACLE_BOUNDARY_KERNEL_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/KernelConfig.hpp>
#include <Simulator/Compute/KernelThread.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>
#include <Simulator/Fluids/Kernels/Helpers.hpp>
#include <Simulator/Fluids/Obstacles/SphereCollider.hpp>
#include <Simulator/Utility/Containers/FixedArray.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint Order, typename LocationTag>
    struct ObstacleBoundaryKernel
    {
        using Config = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;

        using Thread  = Compute::KernelThread<Order>;
        using Domain  = FluidDomain<Order>;
        using Coords  = FloatN<Order>;
        using Index   = IntN<Order>;
        using Helpers = Helpers<Order>;

        template <typename Collider,
                  typename BoundaryAccessor,
                  typename BoundaryDistanceAccessor,
                  typename BoundaryVelocityAccessor>
        static HF_HDINLINE void kernel(Thread                       thread,
                                       Domain                       dom,
                                       Index                        subRegionOffset,
                                       Index                        subRegionSize,
                                       BoundaryAccessor             boundaryField,
                                       BoundaryDistanceAccessor     boundaryDistanceField,
                                       BoundaryVelocityAccessor     boundaryVelocityField,
                                       Collider                     collider,
                                       float                        narrowBandThreshold)
        {
            if (any(greaterThanEqual(thread.index, subRegionSize)))
                return;

            const Index index = subRegionOffset + thread.index;

            // Determine cell position. Obtain current boundary state and distance.

            const Coords pos = dom.getCellPosition(index);
            const float dist = boundaryDistanceField.getValue(index);

            // Compute distance field at the cell.

            float distToCollider; Coords velocity;
            collider.computeDistanceAndVelocity(pos, distToCollider, velocity);

            // If the current distance is the lowest, store obstacle information. 

            if (distToCollider < dist && distToCollider < narrowBandThreshold)
            {
                boundaryDistanceField.setValue(index, distToCollider);
                boundaryVelocityField[0].setValue(index, velocity.x);
                boundaryVelocityField[1].setValue(index, velocity.y);
                boundaryVelocityField[2].setValue(index, velocity.z);

                if (distToCollider < 0.0f)
                    boundaryField.setValue(index, 1);
            }
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUIDS_OBSTACLE_BOUNDARY_KERNEL_HPP */
