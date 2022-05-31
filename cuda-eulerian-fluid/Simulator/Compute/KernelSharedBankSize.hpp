#ifndef HF_SIMULATOR_COMPUTE_KERNEL_SHARED_BANK_SIZE_HPP
#define HF_SIMULATOR_COMPUTE_KERNEL_SHARED_BANK_SIZE_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct KernelSharedBankSize
    {
        /**
         * \brief Use default bank size.
         */
        Default = cudaSharedMemBankSizeDefault,

        /**
         * \brief Use banks of 4 bytes.
         */
        FourBytes = cudaSharedMemBankSizeFourByte,

        /**
         * \brief Use banks of 8 bytes.
         */
        EightBytes = cudaSharedMemBankSizeEightByte
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_KERNEL_SHARED_BANK_SIZE_HPP */
