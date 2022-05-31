#ifndef HF_SIMULATOR_SAMPLING_KERNEL_HPP
#define HF_SIMULATOR_SAMPLING_KERNEL_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/KernelConfig.hpp>
#include <Simulator/Compute/KernelThread.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>
#include <Simulator/Fluids/Kernels/Helpers.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint Order, typename LocationTag>
    struct SamplingKernel;

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename LocationTag>
    struct SamplingKernel<1, LocationTag>
    {
        using Config = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;

        using Thread  = Compute::KernelThread1;
        using Domain  = FluidDomain3;
        using Coords  = Float3;
        using Index   = Int3;
        using Helpers = Helpers<3>;

        template <typename PointsAccessor,
                  typename ValueSampler,
                  typename DataAccessor>
        static HF_HDINLINE void kernel(Thread         thread,
                                       Domain         dom,
                                       PointsAccessor points,
                                       ValueSampler   valueField,
                                       DataAccessor   values,
                                       int            numPoints)
        {
            if (thread.index[0] >= numPoints)
                return;

            Float3 pos = points.getValue(thread.index);

            const auto value = valueField.getValue(dom.getCellCoords(pos));

            values.setValue(thread.index, value);
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_SAMPLING_KERNEL_HPP */