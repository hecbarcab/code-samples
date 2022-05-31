#ifndef HF_SIMULATOR_COMPUTE_KERNEL_THREAD_HPP
#define HF_SIMULATOR_COMPUTE_KERNEL_THREAD_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    /**
     * \brief Structure containing the information of the thread being executed.
     * \tparam Order Order (number of dimensions) of the kernel.
     */
    template <uint Order>
    struct KernelThread
    {
    public:
        using Index = IntN<Order>;
        using Dims  = IntN<Order>;

    public:
        HF_HDINLINE KernelThread(const Index& localIndex,
                                 const Index& blockIndex,
                                 const Dims& blockDims)
            : index(localIndex + blockDims * blockIndex)
            , localIndex(localIndex)
            , blockIndex(blockIndex)
            , blockDims(blockDims)
        {
        }

    public:
        /**
         * \brief Index of the current thread.
         */
        const Index index;

        /**
         * \brief Index of the current thread within the block.
         */
        const Index localIndex;

        /**
         * \brief Index of the current block.
         */
        const Index blockIndex;

        /**
         * \brief Dimensions of the block.
         */
        const Dims blockDims;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using KernelThread1 = KernelThread<1>;
    using KernelThread2 = KernelThread<2>;
    using KernelThread3 = KernelThread<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_KERNEL_THREAD_HPP */
