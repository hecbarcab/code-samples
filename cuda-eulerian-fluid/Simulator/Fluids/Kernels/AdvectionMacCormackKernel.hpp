#ifndef HF_SIMULATOR_FLUIDS_ADVECTION_MACCORMACK_HPP
#define HF_SIMULATOR_FLUIDS_ADVECTION_MACCORMACK_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/KernelConfig.hpp>
#include <Simulator/Compute/KernelThread.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>
#include <Simulator/Fluids/Kernels/Helpers.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint Order, typename LocationTag>
    struct AdvectionMacCormackKernel;

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename LocationTag>
    struct AdvectionMacCormackKernel<3, LocationTag>
    {
        using Config = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;

        using Thread  = Compute::KernelThread3;
        using Domain  = FluidDomain3;
        using Coords  = Float3;
        using Index   = Int3;
        using Helpers = Helpers<3>;

        template <typename ValueSampler,
                  typename ValueConstAccessor,
                  typename VelocityAccessor,
                  typename ValueAccessor>
        static HF_HDINLINE void kernel(Thread             thread,
                                       Domain             dom,
                                       ValueSampler       valueNField,
                                       ValueConstAccessor valueNHatField,
                                       VelocityAccessor   velocityField,
                                       ValueAccessor      valueNP1Field,
                                       float              timestep)
        {
            using Value = typename ValueAccessor::Value;

            if (any(greaterThanEqual(thread.index, dom.getDims())))
                return;

            // Use standard semi-lagrangian on the edges.

            const Int3 index = thread.index;
            
            // Perform MacCormack advection.

            const Value valueN = valueNField.getValue(Coords(index));
            const Value valueNHat = valueNHatField.getValue(index);
            const Value valueNP1Hat = valueNP1Field.getValue(index);
            Value value = valueNP1Hat + 0.5f * (valueN - valueNHat);

            // Ensure the result is unconditionally stable by clamping the
            // the resulting value to the range of values that we would obtain
            // with Semi-Lagrangian.

            const Coords pos = dom.getCellPosition(index);
            const Coords vel = Helpers::getStaggeredVectorAtCell(velocityField, index);
            const Coords prevCoords = dom.getCellCoords(pos - vel * timestep);
            const Index minIndex = dom.getCellIndex(floor(prevCoords));
            const Index maxIndex = dom.getCellIndex(ceil(prevCoords));

            const Value values[8] =
            {
                valueNField.getValue(Coords(minIndex.x, minIndex.y, minIndex.z)),
                valueNField.getValue(Coords(maxIndex.x, minIndex.y, minIndex.z)),
                valueNField.getValue(Coords(minIndex.x, maxIndex.y, minIndex.z)),
                valueNField.getValue(Coords(maxIndex.x, maxIndex.y, minIndex.z)),
                valueNField.getValue(Coords(minIndex.x, minIndex.y, maxIndex.z)),
                valueNField.getValue(Coords(maxIndex.x, minIndex.y, maxIndex.z)),
                valueNField.getValue(Coords(minIndex.x, maxIndex.y, maxIndex.z)),
                valueNField.getValue(Coords(maxIndex.x, maxIndex.y, maxIndex.z))
            };

            const Value minValue = min(min(min(values[0], values[1]), min(values[2], values[3])),
                                       min(min(values[4], values[5]), min(values[6], values[7])));

            const Value maxValue = max(max(max(values[0], values[1]), max(values[2], values[3])),
                                       max(max(values[4], values[5]), max(values[6], values[7])));

            value = clamp(value, minValue, maxValue);

            // Done!

            valueNP1Field.setValue(index, value);
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUIDS_ADVECTION_MACCORMACK_HPP */
