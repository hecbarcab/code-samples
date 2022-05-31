#ifndef HF_SIMULATOR_COMPUTE_KERNEL_CONFIG_HPP
#define HF_SIMULATOR_COMPUTE_KERNEL_CONFIG_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/KernelBlockDims.hpp>
#include <Simulator/Compute/KernelCachePreference.hpp>
#include <Simulator/Compute/KernelSharedBankSize.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    /**
     * \brief Structure containing the configuration of the kernel.
     */
    template <KernelBlockDims       _BlockDims,
              KernelCachePreference _CachePreference = KernelCachePreference::None,
              KernelSharedBankSize  _SharedBankSize  = KernelSharedBankSize::Default>
    struct KernelConfig
    {
    public:
        static constexpr KernelBlockDims       BlockDims       = _BlockDims;
        static constexpr KernelCachePreference CachePreference = _CachePreference;
        static constexpr KernelSharedBankSize  SharedBankSize  = _SharedBankSize;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_KERNEL_CONFIG_HPP */
