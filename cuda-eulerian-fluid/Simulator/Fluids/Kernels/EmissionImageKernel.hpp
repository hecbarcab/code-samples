#ifndef HF_SIMULATOR_FLUIDS_EMISSION_IMAGE_KERNEL_HPP
#define HF_SIMULATOR_FLUIDS_EMISSION_IMAGE_KERNEL_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/KernelConfig.hpp>
#include <Simulator/Compute/KernelThread.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint Order, typename LocationTag>
    struct EmissionImageKernel;

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename LocationTag>
    struct EmissionImageKernel<3, LocationTag>
    {
        using Config = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;
        
        using Thread = Compute::KernelThread3;
        using Index  = Int3;
        using Coords = Float3;

        template <typename InkAccessor,
                  typename ImageAccessor>
        static HF_HDINLINE void kernel(Thread        thread,
                                       InkAccessor   inkField,
                                       Index         offset,
                                       Int2          size,
                                       int           extrusion,
                                       ImageAccessor imageField)
        {
            if (any(greaterThan(Int2(thread.index.xy), size)) && thread.index.z > extrusion)
                return;

            Float4 image = Float4(imageField.getValue(thread.index)) / 255.0f;
            inkField.setValue(thread.index + offset, image);
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUIDS_EMISSION_IMAGE_KERNEL_HPP */
