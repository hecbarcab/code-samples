#ifndef HF_SIMULATOR_COMPUTE_KERNEL_BLOCK_DIMS_HPP
#define HF_SIMULATOR_COMPUTE_KERNEL_BLOCK_DIMS_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct KernelBlockDims
    {
        /**
         * \brief Indicates that the block dimensions should be automatically selected to try
         *        to maximize occupancy.
         */
        Inferred,

        /**
         * \brief Indicates that the block dimensions are manually specified by the kernel.
         *        Note that when using this mode, the kernel must implement a method called
         *        "getBlockDims" specifying the block dimensions.
         */
        Explicit
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_KERNEL_BLOCK_DIMS_HPP */
