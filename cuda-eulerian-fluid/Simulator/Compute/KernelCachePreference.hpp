#ifndef HF_SIMULATOR_COMPUTE_KERNEL_CACHE_PREFERENCE_HPP
#define HF_SIMULATOR_COMPUTE_KERNEL_CACHE_PREFERENCE_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct KernelCachePreference
    {
        /**
         * \brief Default function cache configuration, no preference.
         */
        None = cudaFuncCachePreferNone,

        /**
         * \brief Prefer larger shared memory and smaller L1 cache.
         */
        Shared = cudaFuncCachePreferShared,
        
        /**
         * \brief Prefer larger L1 cache and smaller shared memory.
         */
        L1 = cudaFuncCachePreferL1,

        /**
         * \brief Prefer equal size L1 cache and shared memory.
         */
        Equal = cudaFuncCachePreferEqual
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_KERNEL_CACHE_PREFERENCE_HPP */
