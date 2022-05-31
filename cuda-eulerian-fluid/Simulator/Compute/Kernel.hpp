#ifndef HF_SIMULATOR_COMPUTE_KERNEL_HPP
#define HF_SIMULATOR_COMPUTE_KERNEL_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/Location.hpp>
#include <Simulator/Compute/KernelBlockDims.hpp>
#include <Simulator/Compute/KernelParams.hpp>
#include <Simulator/Compute/KernelThread.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    namespace Kernel
    {
        //─────────────────────────────────────────────────────────────────────────────────────────

        namespace Detail
        {
            //───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ─

            /**
             * \brief Executes the given 1D kernel on the host, emulating the CUDA kernel execution model.
             * \tparam Kernel Kernel to execute.
             * \tparam Args Type of the arguments to the kernel.
             * \param params Kernel execution parameters. Indicated no. of threads, blocks, and so on.
             * \param args Arguments to the kernel.
             */
            template <typename Kernel, typename... Args>
            void hostKernel1(const KernelParams1& params, Args... args)
            {
                // TODO: Use OMP for host-side parallelism.
                for (int blockX = 0; blockX < params.blockCount[0]; blockX++)
                for (int x = 0; x < params.blockDims[0]; ++x)
                {
                    Int1 localIndex(x);
                    Int1 blockIndex(blockX);

                    const KernelThread1 thread(localIndex, blockIndex, params.blockDims);
                    Kernel::kernel(thread, args...);
                }
            }

            /**
             * \brief Executes the given 2D kernel on the host, emulating the CUDA kernel execution model.
             * \tparam Kernel Kernel to execute.
             * \tparam Args Type of the arguments to the kernel.
             * \param params Kernel execution parameters. Indicated no. of threads, blocks, and so on.
             * \param args Arguments to the kernel.
             */
            template <typename Kernel, typename... Args>
            void hostKernel2(const KernelParams2& params, Args... args)
            {
                // TODO: Use OMP for host-side parallelism.
                for (int blockY = 0; blockY < params.blockCount[1]; blockY++)
                for (int blockX = 0; blockX < params.blockCount[0]; blockX++)
                for (int y = 0; y < params.blockDims[1]; ++y)
                for (int x = 0; x < params.blockDims[0]; ++x)
                {
                    Int2 localIndex(x, y);
                    Int2 blockIndex(blockX, blockY);
                    
                    const KernelThread2 thread(localIndex, blockIndex, params.blockDims);
                    Kernel::kernel(thread, args...);
                }
            }

            /**
             * \brief Executes the given 3D kernel on the host, emulating the CUDA kernel execution model.
             * \tparam Kernel Kernel to execute.
             * \tparam Args Type of the arguments to the kernel.
             * \param params Kernel execution parameters. Indicated no. of threads, blocks, and so on.
             * \param args Arguments to the kernel.
             */
            template <typename Kernel, typename... Args>
            void hostKernel3(const KernelParams3& params, Args... args)
            {
                // TODO: Use OMP for host-side parallelism.
                for (int blockZ = 0; blockZ < params.blockCount[2]; blockZ++)
                for (int blockY = 0; blockY < params.blockCount[1]; blockY++)
                for (int blockX = 0; blockX < params.blockCount[0]; blockX++)
                for (int z = 0; z < params.blockDims[2]; ++z)
                for (int y = 0; y < params.blockDims[1]; ++y)
                for (int x = 0; x < params.blockDims[0]; ++x)
                {
                    Int3 localIndex(x, y, z);
                    Int3 blockIndex(blockX, blockY, blockZ);

                    const KernelThread3 thread(localIndex, blockIndex, params.blockDims);
                    Kernel::kernel(thread, args...);
                }
            }

            //───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ─

            /**
             * \brief Executes the given 1D kernel on the device.
             * \tparam Kernel Kernel to execute.
             * \tparam Args Type of the arguments to the kernel.
             * \param args Arguments to the kernel.
             */
            template <typename Kernel, typename... Args>
            __global__ void deviceKernel1(Args... args)
            {
                const Int1 localIndex  = HF_LOCAL_THREAD_IDX_1;
                const Int1 blockIndex  = HF_BLOCK_IDX_1;
                const Int1 blockDims = HF_BLOCK_DIMS_1;
                
                const KernelThread1 thread(localIndex, blockIndex, blockDims);
                Kernel::kernel(thread, args...);
            }

            /**
             * \brief Executes the given 2D kernel on the device.
             * \tparam Kernel Kernel to execute.
             * \tparam Args Type of the arguments to the kernel.
             * \param args Arguments to the kernel.
             */
            template <typename Kernel, typename... Args>
            __global__ void deviceKernel2(Args... args)
            {
                const Int2 localIndex = HF_LOCAL_THREAD_IDX_2;
                const Int2 blockIndex = HF_BLOCK_IDX_2;
                const Int2 blockDims = HF_BLOCK_DIMS_2;

                const KernelThread2 thread(localIndex, blockIndex, blockDims);
                Kernel::kernel(thread, args...);
            }

            /**
             * \brief Executes the given 3D kernel on the device.
             * \tparam Kernel Kernel to execute.
             * \tparam Args Type of the arguments to the kernel.
             * \param args Arguments to the kernel.
             */
            template <typename Kernel, typename... Args>
            __global__ void deviceKernel3(Args... args)
            {
                const Int3 localIndex = HF_LOCAL_THREAD_IDX_3;
                const Int3 blockIndex = HF_BLOCK_IDX_3;
                const Int3 blockDims = HF_BLOCK_DIMS_3;

                const KernelThread3 thread(localIndex, blockIndex, blockDims);
                Kernel::kernel(thread, args...);
            }

            //───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ─

            template <uint Order, typename Kernel, typename... Args>
            struct KernelSelector;

            template <typename Kernel, typename ...Args>
            struct KernelSelector<1, Kernel, Args...>
            {
                static void execute(const Location::HostTag&, const KernelParams1& params, Args... args)
                {
                    hostKernel1<Kernel>(params, args...);
                }

                static void execute(const Location::DeviceTag&, const KernelParams1& params, Args... args)
                {
                    using KernelCfg = typename Kernel::Config;

                    auto func = &deviceKernel1<Kernel, Args...>;
                    cudaFuncSetCacheConfig(func, static_cast<cudaFuncCache>(KernelCfg::CachePreference));
                    cudaFuncSetSharedMemConfig(func, static_cast<cudaSharedMemConfig>(KernelCfg::SharedBankSize));

                    const dim3 blockCount = castCudaDims(params.blockCount);
                    const dim3 blockDims = castCudaDims(params.blockDims);
                    func<<<blockCount, blockDims>>>(args...);

                    if (HF_SYNCHRONIZE_AT_KERNELS)
                    {
                        HF_CUDA(DeviceSynchronize());
                        HF_CUDA(GetLastError());
                    }
                }
            };

            template <typename Kernel, typename ...Args>
            struct KernelSelector<2u, Kernel, Args...>
            {
                static void execute(const Location::HostTag&, const KernelParams2& params, Args... args)
                {
                    hostKernel2<Kernel>(params, args...);
                }

                static void execute(const Location::DeviceTag&, const KernelParams2& params, Args... args)
                {
                    using KernelCfg = typename Kernel::Config;

                    auto func = &deviceKernel2<Kernel, Args...>;
                    cudaFuncSetCacheConfig(func, static_cast<cudaFuncCache>(KernelCfg::CachePreference));
                    cudaFuncSetSharedMemConfig(func, static_cast<cudaSharedMemConfig>(KernelCfg::SharedBankSize));

                    const dim3 blockCount = castCudaDims(params.blockCount);
                    const dim3 blockDims = castCudaDims(params.blockDims);
                    func<<<blockCount, blockDims>>>(args...);

                    if (HF_SYNCHRONIZE_AT_KERNELS)
                    {
                        HF_CUDA(DeviceSynchronize());
                        HF_CUDA(GetLastError());
                    }
                }
            };

            template <typename Kernel, typename ...Args>
            struct KernelSelector<3u, Kernel, Args...>
            {
                static void execute(const Location::HostTag&, const KernelParams3& params, Args... args)
                {
                    hostKernel3<Kernel>(params, args...);
                }

                static void execute(const Location::DeviceTag&, const KernelParams3& params, Args... args)
                {
                    using KernelCfg = typename Kernel::Config;

                    auto func = &deviceKernel3<Kernel, Args...>;
                    cudaFuncSetCacheConfig(func, static_cast<cudaFuncCache>(KernelCfg::CachePreference));
                    cudaFuncSetSharedMemConfig(func, static_cast<cudaSharedMemConfig>(KernelCfg::SharedBankSize));

                    const dim3 blockCount = castCudaDims(params.blockCount);
                    const dim3 blockDims = castCudaDims(params.blockDims);
                    func<<<blockCount, blockDims>>>(args...);

                    if (HF_SYNCHRONIZE_AT_KERNELS)
                    {
                        HF_CUDA(DeviceSynchronize());
                        HF_CUDA(GetLastError());
                    }
                }
            };

            //───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ─

            /**
             * \brief Determines the optimal block size for the given 1D kernel according to the
             *        host capabilities and the kernel itself.
             * \tparam Kernel Kernel to execute.
             * \param threadCount Kernel execution parameters.
             * \return Optimal block size.
             */
            template <typename Kernel>
            Int1 inferBlockDimsOnHost(const Int1& threadCount)
            {
                // TODO: When implementing OMP, select block size depending on system capabilities
                //       such as no. of threads, etc.
                return Int1(1024);
            }

            /**
             * \brief Determines the optimal block size for the given 2D kernel according to the
             *        host capabilities and the kernel itself.
             * \tparam Kernel Kernel to execute.
             * \param threadCount Kernel execution parameters.
             * \return Optimal block size.
             */
            template <typename Kernel>
            Int2 inferBlockDimsOnHost(const Int2& threadCount)
            {
                // TODO: When implementing OMP, select block size depending on system capabilities
                //       such as no. of threads, etc.
                return Int2(32, 32);
            }

            /**
             * \brief Determines the optimal block size for the given 3D kernel according to the
             *        host capabilities and the kernel itself.
             * \tparam Kernel Kernel to execute.
             * \param threadCount Kernel execution parameters.
             * \return Optimal block size.
             */
            template <typename Kernel>
            Int3 inferBlockDimsOnHost(const Int3& threadCount)
            {
                // TODO: When implementing OMP, select block size depending on system capabilities
                //       such as no. of threads, etc.
                return Int3(16, 16, 4);
            }

            /**
             * \brief Determines the optimal block size for the given 1D kernel according to the
             *        device capabilities and the kernel itself.
             * \tparam Kernel Kernel to execute.
             * \param threadCount Kernel execution parameters.
             * \return Optimal block size.
             */
            template <typename Kernel, typename... Args>
            Int1 inferBlockDimsOnDevice(const Int1& threadCount)
            {
                int blockSize;   // The launch configurator returned block size 
                int minGridSize; // The minimum grid size needed to achieve the maximum occupancy for a full device launch.

                // Determine block size that achieves maximum potential occupancy for the kernel.
                // Ignore minimum grid size.

                cudaOccupancyMaxPotentialBlockSize(&minGridSize, &blockSize, &deviceKernel1<Kernel, Args...>, 0, 0);

                return Int1(blockSize);
            }

            /**
             * \brief Determines the optimal block size for the given 2D kernel according to the
             *        device capabilities and the kernel itself.
             * \tparam Kernel Kernel to execute.
             * \param threadCount Kernel execution parameters.
             * \return Optimal block size.
             */
            template <typename Kernel, typename... Args>
            Int2 inferBlockDimsOnDevice(const Int2& threadCount)
            {
                int blockSize;   // The launch configurator returned block size 
                int minGridSize; // The minimum grid size needed to achieve the 
                                 // maximum occupancy for a full device launch.

                // Determine block size that achieves maximum potential occupancy for the kernel.
                // Ignore minimum grid size.

                cudaOccupancyMaxPotentialBlockSize(&minGridSize, &blockSize, &deviceKernel2<Kernel, Args...>, 0, 0);

                // Find nearest block size configuration in 2D.

                const int blockSizeX = max(1, int(upperPowerOfTwo(pow(float(blockSize), 1.0f / 3.0f))));
                const int blockSizeY = max(1, int(lowerPowerOfTwo(float(blockSize) / blockSizeX)));

                return Int2(blockSizeX, blockSizeY);
            }

            /**
             * \brief Determines the optimal block size for the given 3D kernel according to the
             *        device capabilities.
             * \tparam Kernel Kernel to execute.
             * \param threadCount Kernel execution parameters.
             * \return Optimal block size.
             */
            template <typename Kernel, typename... Args>
            Int3 inferBlockDimsOnDevice(const Int3& threadCount)
            {
                int blockSize;   // The launch configurator returned block size 
                int minGridSize; // The minimum grid size needed to achieve the 
                                 // maximum occupancy for a full device launch.

                // Determine block size that achieves maximum potential occupancy for the kernel.
                // Ignore minimum grid size.

                cudaOccupancyMaxPotentialBlockSize(&minGridSize, &blockSize, &deviceKernel3<Kernel, Args...>, 0, 0);

                // Find nearest block size configuration in 3D.

                const int blockSizeXY = max(1, int(upperPowerOfTwo(pow(float(blockSize), 1.0f / 3.0f))));
                const int blockSizeZ  = max(1, int(lowerPowerOfTwo(float(blockSize) / (blockSizeXY * blockSizeXY))));

                return Int3(blockSizeXY, blockSizeXY, blockSizeZ);
            }

            //───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ─

            template <KernelBlockDims BlockDims, uint Order, typename Kernel, typename... Args>
            struct KernelBlockDimsSelector;

            template <uint Order, typename Kernel, typename... Args>
            struct KernelBlockDimsSelector<KernelBlockDims::Inferred, Order, Kernel, Args...>
            {
                static KernelParams<Order> get(const Location::HostTag&, const IntN<Order>& threadCount)
                {
                    const auto blockDims = inferBlockDimsOnHost<Kernel>(threadCount);
                    return KernelParams<Order>(threadCount, blockDims);
                }

                static KernelParams<Order> get(const Location::DeviceTag&, const IntN<Order>& threadCount)
                {
                    const auto blockDims = inferBlockDimsOnDevice<Kernel, Args...>(threadCount);
                    return KernelParams<Order>(threadCount, blockDims);
                }
            };

            template <uint Order, typename Kernel, typename... Args>
            struct KernelBlockDimsSelector<KernelBlockDims::Explicit, Order, Kernel, Args...>
            {
                template <typename LocationTag>
                static KernelParams<Order> get(const LocationTag&, const IntN<Order>& threadCount)
                {
                    const auto blockDims = Kernel::getBlockDims();
                    return KernelParams<Order>(threadCount, blockDims);
                }
            };

            //───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ─
        }

        //─────────────────────────────────────────────────────────────────────────────────────────

        /**
         * \brief Executes the given kernel at the desired location and with the supplied parameters.
         * \tparam Order Order (dimensions) of the kernel to execute.
         * \tparam Kernel Type of the kernel to execute.
         * \tparam Args Type of the arguments supplied to the kernel.
         * \param params Kernel execution parameters.
         * \param args Arguments supplied to the kernel.
         */
        template <uint Order, template <uint, typename> class Kernel, typename... Args>
        void execute(const Location::DeviceTag&, const KernelParams<Order>& params, Args... args)
        {
            static_assert(Order >= 1 && Order <= 3, "Only 1D, 2D and 3D kernels may be executed.");
        
            using KernelType = Kernel<Order, Location::DeviceTag>;
            using KernelSelector = Detail::KernelSelector<Order, KernelType, Args...>;
        
            KernelSelector::execute(Location::Device, params, args...);
        }

        /**
         * \brief Executes the given kernel at the desired location and with the supplied parameters.
         * \tparam Order Order (dimensions) of the kernel to execute.
         * \tparam Kernel Type of the kernel to execute.
         * \tparam Args Type of the arguments supplied to the kernel.
         * \param threadCount No. of threads to launch.
         * \param args Arguments supplied to the kernel.
         */
        template <uint Order, template <uint, typename> class Kernel, typename... Args>
        void execute(const Location::DeviceTag&, const IntN<Order>& threadCount, Args... args)
        {
            static_assert(Order >= 1 && Order <= 3, "Only 1D, 2D and 3D kernels may be executed.");

            using KernelType = Kernel<Order, Location::DeviceTag>;
            using KernelCfg = typename KernelType::Config;
            using KernelParams = KernelParams<Order>;
            using KernelSelector = Detail::KernelSelector<Order, KernelType, Args...>;
            using KernelBlockDims = Detail::KernelBlockDimsSelector<KernelCfg::BlockDims, Order, KernelType, Args...>;

            const KernelParams params = KernelBlockDims::get(Location::Device, threadCount);
            KernelSelector::execute(Location::Device, params, args...);
        }

        /**
         * \brief Executes the given kernel at the desired location and with the supplied parameters.
         * \tparam Order Order (dimensions) of the kernel to execute.
         * \tparam Kernel Type of the kernel to execute.
         * \tparam Args Type of the arguments supplied to the kernel.
         * \param params Kernel execution parameters.
         * \param args Arguments supplied to the kernel.
         */
        template <uint Order, template <uint, typename> class Kernel, typename... Args>
        void execute(const Location::HostTag&, const KernelParams<Order>& params, Args... args)
        {
            static_assert(Order >= 1 && Order <= 3, "Only 1D, 2D and 3D kernels may be executed.");
        
            using KernelType = Kernel<Order, Location::HostTag>;
            using KernelSelector = Detail::KernelSelector<Order, KernelType, Args...>;
        
            KernelSelector::execute(Location::Host, params, args...);
        }

        /**
         * \brief Executes the given kernel at the desired location and with the supplied parameters.
         * \tparam Order Order (dimensions) of the kernel to execute.
         * \tparam Kernel Type of the kernel to execute.
         * \tparam Args Type of the arguments supplied to the kernel.
         * \param threadCount No. of threads to launch.
         * \param args Arguments supplied to the kernel.
         */
        template <uint Order, template <uint, typename> class Kernel, typename... Args>
        void execute(const Location::HostTag&, const IntN<Order>& threadCount, Args... args)
        {
            static_assert(Order >= 1 && Order <= 3, "Only 1D, 2D and 3D kernels may be executed.");

            using KernelType = Kernel<Order, Location::HostTag>;
            using KernelCfg = typename KernelType::Config;
            using KernelParams = KernelParams<Order>;
            using KernelSelector = Detail::KernelSelector<Order, KernelType, Args...>;
            using KernelBlockDims = Detail::KernelBlockDimsSelector<KernelCfg::BlockDims, Order, KernelType, Args...>;

            const KernelParams params = KernelBlockDims::get(Location::Host, threadCount);
            KernelSelector::execute(Location::Host, params, args...);
        }

        //───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───
    }
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_KERNEL_HPP */

