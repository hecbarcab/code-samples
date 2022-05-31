#ifndef HF_SIMULATOR_FLUIDS_EMISSION_KERNEL_HPP
#define HF_SIMULATOR_FLUIDS_EMISSION_KERNEL_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/KernelConfig.hpp>
#include <Simulator/Compute/KernelThread.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint Order, typename LocationTag>
    struct EmissionKernel
    {
        using Config = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;
        
        using Thread = Compute::KernelThread<Order>;
        using Domain = FluidDomain<Order>;
        using Coords = FloatN<Order>;

        static HF_HDINLINE float computeEmitterWeight(const Coords& offs,
                                                      float         radius,
                                                      float         falloff)
        {
            const float dst = max(0.0f, radius - length(offs));
            return pow(dst, falloff);
        }

        template <typename InkAccessor,
                  typename VelocityAccessor>
        static HF_HDINLINE void kernel(Thread           thread,
                                       Domain           dom,
                                       InkAccessor      inkField,
                                       VelocityAccessor velocityField,
                                       Coords           center,
                                       float            radius,
                                       float            falloff,
                                       Coords           velocity,
                                       Coords           angularVelocity,
                                       Float4           color)
        {
            // Setup ink.

            if (all(lessThan(thread.index, dom.getDims())))
            {
                const Coords pos = dom.getCellPosition(thread.index);
                const Coords offs = pos - center;
                const float weight = computeEmitterWeight(offs, radius, falloff);
                
                Float4 ink = inkField.getValue(thread.index);

                ink = mix(ink, color, weight);

                inkField.setValue(thread.index, ink);                
            }
                
            // Setup velocity field

            for (uint axis = 0; axis < Order; ++axis)
            {
                if (any(greaterThanEqual(thread.index, dom.getDimsOfFaceGrid(axis))))
                    continue;

                const Coords pos = dom.getFacePosition(thread.index, axis);
                const Coords offs = pos - center;
                const float weight = computeEmitterWeight(offs, radius, falloff);

                // vx = wy oz - wz oy
                // vy = wz ox - wx oz
                // vz = wx oy - wy ox

                float vel = velocity[axis]
                          + angularVelocity[(axis + 1) % 3] * offs[(axis + 2) % 3]
                          - angularVelocity[(axis + 2) % 3] * offs[(axis + 1) % 3];

                vel = mix(velocityField[axis].getValue(thread.index), vel, weight);

                velocityField[axis].setValue(thread.index, vel);
            }
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUIDS_EMISSION_KERNEL_HPP */
