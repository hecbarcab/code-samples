#ifndef HF_SIMULATOR_FLUIDS_VELOCITY_BOUNDARY_PROJECTION_HPP
#define HF_SIMULATOR_FLUIDS_VELOCITY_BOUNDARY_PROJECTION_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/KernelThread.hpp>
#include <Simulator/Compute/KernelBlockDims.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>
#include <Simulator/Fluids/FluidDomainBounds.hpp>
#include <Simulator/Fluids/FluidDomainBoundsVelocity.hpp>
#include <Simulator/Fluids/Kernels/Helpers.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint Order, typename LocationTag>
    struct VelocityBoundaryProjectionKernel
    {
        using Config = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;

        using Thread               = Compute::KernelThread<Order>;
        using Domain               = FluidDomain<Order>;
        using DomainBounds         = FluidDomainBounds<Order>;
        using DomainBoundsVelocity = FluidDomainBoundsVelocity<Order>;
        using Coords               = FloatN<Order>;
        using Index                = IntN<Order>;
        using Helpers              = Helpers<Order>;

        template <typename BoundaryConstAccessor,
                  typename BoundaryVelocityConstAccessor,
                  typename BoundaryDistanceConstAccessor,
                  typename VelocityAccessor>
        static HF_HDINLINE void kernel(Thread                        thread,
                                       Domain                        dom,
                                       DomainBounds                  domBounds,
                                       DomainBoundsVelocity          domVelocity,
                                       BoundaryConstAccessor         boundaryField,
                                       BoundaryDistanceConstAccessor boundaryDistanceField,
                                       BoundaryVelocityConstAccessor boundaryVelocityField,
                                       VelocityAccessor              velocityField)
        {
            for (uint axis = 0; axis < Order; ++axis)
            {
                if (any(greaterThanEqual(thread.index, dom.getDimsOfFaceGrid(axis))))
                    continue;

                // Get velocity for the current face.

                Coords vel = Helpers::getStaggeredVectorAtFace(velocityField, thread.index, axis);

                // Get coordinates of closest field faces. Determine the type of boundary they are.

                Index prevIndex = thread.index; --prevIndex[axis];
                Index nextIndex = thread.index; 

                const FluidBounds prevBoundary = Helpers::getBoundaryAtCell(dom, domBounds, boundaryField, prevIndex);
                const FluidBounds nextBoundary = Helpers::getBoundaryAtCell(dom, domBounds, boundaryField, nextIndex);

                // If either one of the boundaries is solid, project velocities to satisfy solid boundary conditions.
                // n·(u - uₛ) = 0

                if (prevBoundary == FluidBounds::Solid)
                {
                    const Coords boundaryVel = Helpers::getBoundaryVelocityAtCell(dom, domVelocity, boundaryVelocityField, prevIndex);
                    const Coords boundaryNor = Helpers::getBoundaryNormalAtCell(dom, boundaryDistanceField, prevIndex);
                    vel = vel + boundaryVel - boundaryNor * dot(vel, boundaryNor);
                }

                if (nextBoundary == FluidBounds::Solid)
                {
                    const Coords boundaryVel = Helpers::getBoundaryVelocityAtCell(dom, domVelocity, boundaryVelocityField, nextIndex);
                    const Coords boundaryNor = Helpers::getBoundaryNormalAtCell(dom, boundaryDistanceField, nextIndex);
                    vel = vel + boundaryVel - boundaryNor * dot(vel, boundaryNor);
                }

                // Store new velocities.

                velocityField[axis].setValue(thread.index, vel[axis]);
            }
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUIDS_VELOCITY_BOUNDARY_PROJECTION_HPP */
