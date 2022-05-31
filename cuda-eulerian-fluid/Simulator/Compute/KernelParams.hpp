#ifndef HF_SIMULATOR_COMPUTE_KERNEL_PARAMS_HPP
#define HF_SIMULATOR_COMPUTE_KERNEL_PARAMS_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    /**
     * \brief Structure containing the parameters for the kernel execution.
     * \tparam _Order Order (dimensions) of the kernel to be executed.
     */
    template <uint _Order>
    struct KernelParams
    {
    public:
        static constexpr uint Order = _Order;
        using Dims = IntN<Order>;

    public:
        KernelParams(const Dims& threadCount, const Dims& blockDims)
            : threadCount(threadCount)
            , blockCount((threadCount + blockDims - Dims(1)) / blockDims)
            , blockDims(blockDims)
        {
        }

    public:
        /**
         * \brief Total desired number of threads.
         */
        Dims threadCount;
        
        /**
         * \brief Number of blocks to launch.
         */
        Dims blockCount;

        /**
         * \brief Dimensions (no. of threads) per block.
         */
        Dims blockDims;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using KernelParams1 = KernelParams<1>;
    using KernelParams2 = KernelParams<2>;
    using KernelParams3 = KernelParams<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_KERNEL_PARAMS_HPP */
