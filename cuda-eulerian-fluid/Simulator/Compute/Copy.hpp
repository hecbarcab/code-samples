#ifndef HF_SIMULATOR_COMPUTE_COPY_HPP
#define HF_SIMULATOR_COMPUTE_COPY_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/Kernel.hpp>
#include <Simulator/Compute/KernelConfig.hpp>
#include <Simulator/Compute/KernelParams.hpp>
#include <Simulator/Compute/KernelThread.hpp>
#include <Simulator/Compute/Arrays/Array.hpp>
#include <Simulator/Compute/Buffers/Buffer.hpp>
#include <Simulator/Compute/Surfaces/Surface.hpp>
#include <Simulator/Compute/CopyRegion.hpp>
#include <Simulator/Compute/CopyValueRegion.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    namespace Copy
    {
        namespace Detail
        {
            //───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ─

            /**
             * \brief Copies the given amount of elements from the source pointer to the destination.
             * \tparam T Data type of the source and destination pointer.
             * \param src Source pointer.
             * \param dst Destination pointer.
             * \param count Number of elements to copy.
             */
            template <typename T>
            void copyMemory(const Location::HostTag&, const Location::HostTag&, const T* src, T* dst, uint count)
            {
                HF_CUDA(Memcpy(dst, src, sizeof(T) * count, cudaMemcpyHostToHost));
            }

            /**
             * \brief Copies the given amount of elements from the source pointer to the destination.
             * \tparam T Data type of the source and destination pointer.
             * \param src Source pointer.
             * \param dst Destination pointer.
             * \param count Number of elements to copy.
             */
            template <typename T>
            void copyMemory(const Location::HostTag&, const Location::DeviceTag&, const T* src, T* dst, uint count)
            {
                HF_CUDA(Memcpy(dst, src, sizeof(T) * count, cudaMemcpyHostToDevice));
            }

            /**
             * \brief Copies the given amount of elements from the source pointer to the destination.
             * \tparam T Data type of the source and destination pointer.
             * \param src Source pointer.
             * \param dst Destination pointer.
             * \param count Number of elements to copy.
             */
            template <typename T>
            void copyMemory(const Location::DeviceTag&, const Location::HostTag&, const T* src, T* dst, uint count)
            {
                HF_CUDA(Memcpy(dst, src, sizeof(T) * count, cudaMemcpyDeviceToDevice));
            }

            /**
             * \brief Copies the given amount of elements from the source pointer to the destination.
             * \tparam T Data type of the source and destination pointer.
             * \param src Source pointer.
             * \param dst Destination pointer.
             * \param count Number of elements to copy.
             */
            template <typename T>
            void copyMemory(const Location::DeviceTag&, const Location::DeviceTag&, const T* src, T* dst, uint count)
            {
                HF_CUDA(Memcpy(dst, src, sizeof(T) * count, cudaMemcpyDeviceToDevice));
            }

            /**
             * \brief Copies the given amount of elements from the source pointer to the destination.
             * \tparam LocationTag Type of the tag indicating the source and destinationlocations.
             * \tparam T Data type of the source and destination pointer.
             * \param tag Tag indicating the source and destination locations.
             * \param src Source pointer.
             * \param dst Destination pointer.
             * \param count Number of elements to copy.
             */
            template <typename LocationTag, typename T>
            void copyMemory(const LocationTag& tag, T* src, T* dst, uint count)
            {
                copyMemory(tag, tag, src, dst, count);
            }

            //───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ─

            /**
             * \brief Simple kernel that handles the assignment of an specific value in a data subregion.
             * \tparam Order Order (dimensions) of the kernel.
             * \tparam Location Location where the kernel will be executed.
             */
            template <uint Order, typename Location>
            struct CopyValueKernel
            {
                using Config = KernelConfig<KernelBlockDims::Inferred>;

                using KernelThread    = KernelThread<Order>;
                using CopyValueRegion = CopyValueRegion<Order>;
                
                template <typename SrcValue, typename DstAccessor>
                static HF_HDINLINE void kernel(KernelThread thread, CopyValueRegion region, SrcValue src, DstAccessor dst)
                {
                    if (any(greaterThanEqual(thread.index, region.size))) return;

                    const auto offset = region.offset + thread.index;
                    dst.setValue(offset, src);
                }
            };

            /**
             * \brief Launches the kernel to copy the given value to the specified region.
             * \tparam Order Order (dimensions) of the kernel.
             * \tparam Location Location where the kernel will be executed.
             * \tparam SrcValue Type of the value to copy into the destination.
             * \tparam DstAccessor Type of the accessor to the destination data.
             * \param location Tag indicating the source and destination locations.
             * \param region Region to copy.
             * \param src Value to copy into the 
             * \param dst Accessor to the destination data.
             */
            template <uint Order, typename Location, typename SrcValue, typename DstAccessor>
            void copyValue(const Location& location, const CopyValueRegion<Order>& region, const SrcValue& src, DstAccessor dst)
            {
                Kernel::execute<Order, CopyValueKernel>(location, region.size, region, src, dst);
            }

            //───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ─

            /**
             * \brief Kernel that handles the data transfer of two data subregions.
             * \tparam Order Order (dimensions) of the kernel.
             * \tparam Location Location where the kernel will be executed.
             */
            template <uint Order, typename Location>
            struct CopyRegionKernel
            {
                using Config = KernelConfig<KernelBlockDims::Inferred>;

                using KernelThread = KernelThread<Order>;
                using CopyRegion   = CopyRegion<Order>;

                /**
                 * \brief The kernel itself. Transfers the given region from source to destination.
                 * \tparam SrcAccessor Type of the accessor to the source data.
                 * \tparam DstAccessor Type of the accessor to the destination data.
                 * \param thread Current thread being executed.
                 * \param region Region to copy.
                 * \param src Accessor to the source data.
                 * \param dst Accessor to the destination data.
                 */
                template <typename SrcAccessor, typename DstAccessor>
                static HF_HDINLINE void kernel(KernelThread thread, CopyRegion region, SrcAccessor src, DstAccessor dst)
                {
                    if (any(greaterThanEqual(thread.index, region.size))) return;

                    const auto srcOffset = region.srcOffset + thread.index;
                    const auto dstOffset = region.dstOffset + thread.index;

                    const auto value = src.getValue(srcOffset);
                    dst.setValue(dstOffset, value);
                }
            };

            /**
             * \brief Launches the kernel to copy the specified region.
             * \tparam Order Order (dimensions) of the kernel.
             * \tparam Location Location where the kernel will be executed.
             * \tparam SrcAccessor Type of the accessor to the source data.
             * \tparam DstAccessor Type of the accessor to the destination data.
             * \param location Tag indicating the source and destination locations.
             * \param region Region to copy.
             * \param src Accessor to the source data.
             * \param dst Accessor to the destination data.
             */
            template <uint Order, typename Location, typename SrcAccessor, typename DstAccessor>
            void copyRegion(const Location& location, const CopyRegion<Order>& region, SrcAccessor src, DstAccessor dst)
            {
                Kernel::execute<Order, CopyRegionKernel>(location, region.size, region, src, dst);
            }

            //───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ─

            /**
             * \brief Copies the contents of the given array subregion.
             * \tparam Order Order of the array.
             * \param src Source array object.
             * \param dst Destination array object.
             * \param region Subregion of the array that will be copied.
             */
            template <uint Order>
            void copyArrayRegion(cudaArray_t src, cudaArray_t dst, const CopyRegion<Order>& region)
            {
                cudaMemcpy3DParms params = { nullptr };
                params.srcArray = src;
                params.dstArray = dst;
                params.srcPos = castCudaPos(region.srcOffset);
                params.dstPos = castCudaPos(region.dstOffset);
                params.extent = castCudaExtents(region.size);
                params.kind = cudaMemcpyDeviceToDevice;
                
                HF_CUDA(Memcpy3D(&params));
            }
        }

        //─────────────────────────────────────────────────────────────────────────────────────────

        /**
         * \brief Copies a subregion of the given buffers. If the buffers are not stored at the same
         *        location, temporary buffers will be allocated and an explicit device synchronization
         *        will be performed.
         * \tparam SrcBufferRef Type of the source buffer.
         * \tparam DstBufferRef Type of the destination buffer.
         * \tparam Order Order (dimensions) of the buffers.
         * \param src Source buffer.
         * \param dst Destination buffer.
         * \param region Subregion of the buffer that will be copied.
         */
        template <typename SrcBufferRef, typename DstBufferRef, uint Order>
        void bufferToBuffer(const SrcBufferRef& src, DstBufferRef& dst, const CopyRegion<Order>& region)
        {
            // Inferred buffer parameters.

            using SrcBuffer = typename SrcBufferRef::element_type;
            using DstBuffer = typename DstBufferRef::element_type;
            using SrcValue = typename SrcBuffer::Value;
            using DstValue = typename DstBuffer::Value;
            using SrcIndexer = typename SrcBuffer::Indexer;
            using DstIndexer = typename DstBuffer::Indexer;
            using SrcLocationTag = typename SrcBuffer::LocationTag;
            using DstLocationTag = typename DstBuffer::LocationTag;
            constexpr SrcLocationTag srcLocation = SrcBuffer::Location;
            constexpr DstLocationTag dstLocation = DstBuffer::Location;
            constexpr uint srcOrder = SrcBuffer::Order;
            constexpr uint dstOrder = DstBuffer::Order;

            // Sanitize inputs

            static_assert(std::is_same<SrcValue, DstValue>::value,
                "The data type of the given buffers mismatch.");

            static_assert(srcOrder == Order && dstOrder == Order,
                "The order of the given buffers mismatch.");

            HF_ASSERT(all(greaterThanEqual(src->getDims(), region.srcOffset + region.size)),
                "The given copy region is outside of the bounds of the source buffer.");

            HF_ASSERT(all(greaterThanEqual(dst->getDims(), region.dstOffset + region.size)),
                "The given copy region is outside of the bounds of the destination buffer.");

            // If both buffers are on the same location, simply copy the subregion directly.

            if (std::is_same<SrcLocationTag, DstLocationTag>::value)
            {
                Detail::copyRegion(srcLocation, region, src->getConstAccessor(), dst->getAccessor());
            }

            // Otherwise, allocate intermediate buffers for transferring between the differring
            // locations and copy.

            else
            {
                auto srcTemp = Buffer<SrcLocationTag, Order, SrcValue, DstIndexer>::Create(region.size);
                auto dstTemp = Buffer<DstLocationTag, Order, DstValue, DstIndexer>::Create(region.size);
                
                const CopyRegion<Order> srcRegion(region.srcOffset, UintN<Order>(0), region.size);
                const CopyRegion<Order> dstRegion(UintN<Order>(0), region.dstOffset, region.size);

                Detail::copyRegion(srcLocation, srcRegion, src->getConstAccessor(), srcTemp->getAccessor());
                Detail::copyMemory(srcLocation, dstLocation, srcTemp->getPtr(), dstTemp->getPtr(), srcTemp->getStorage());
                Detail::copyRegion(dstLocation, dstRegion, dstTemp->getConstAccessor(), dst->getAccessor());

                if (HF_SYNCHRONIZE_AT_MEM_TRANSFER)
                {
                    HF_CUDA(DeviceSynchronize());
                }
            }
        }

        /**
         * \brief Copies the contents of the given buffers. If the buffers do not employ the same
         *        indexing method, temporary buffers will be allocated and an explicit device synchronization
         *        will be performed.
         * \tparam SrcBufferRef Type of the source buffer.
         * \tparam DstBufferRef Type of the destination buffer.
         * \param src Source buffer.
         * \param dst Destination buffer.
         */
        template <typename SrcBufferRef, typename DstBufferRef>
        void bufferToBuffer(const SrcBufferRef& src, DstBufferRef& dst)
        {
            // Inferred buffer parameters.

            using SrcBuffer = typename SrcBufferRef::element_type;
            using DstBuffer = typename DstBufferRef::element_type;
            using SrcValue = typename SrcBuffer::Value;
            using DstValue = typename DstBuffer::Value;
            using SrcIndexer = typename SrcBuffer::Indexer;
            using DstIndexer = typename DstBuffer::Indexer;
            using SrcLocationTag = typename SrcBuffer::LocationTag;
            using DstLocationTag = typename DstBuffer::LocationTag;
            constexpr SrcLocationTag srcLocation = SrcBuffer::Location;
            constexpr DstLocationTag dstLocation = DstBuffer::Location;
            constexpr uint srcOrder = SrcBuffer::Order;
            constexpr uint dstOrder = DstBuffer::Order;

            // Sanitize inputs

            static_assert(std::is_same<SrcValue, DstValue>::value,
                "The data type of the given buffers mismatch.");

            static_assert(srcOrder == dstOrder,
                "The order of the given buffers mismatch.");

            HF_ASSERT(src->getDims() == dst->getDims(),
                "Source and destination dimensions mismatch.");

            // If both indexers are of the same type, directly copy whole buffer using cudaMemcpy.

            if (std::is_same<SrcIndexer, DstIndexer>::value)
            {
                const auto count = src->getStorage();
                Detail::copyMemory(srcLocation, dstLocation, src->getPtr(), dst->getPtr(), count);
            }

            // Otherwise, fallback to the region-based method, which's able to handle this very case.

            else
            {
                const CopyRegion<srcOrder> region(src->getDims());
                bufferToBuffer(src, dst, region);
            }
        }

        //───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───

        /**
         * \brief Copies a subregion of the given buffer into the array. If the buffer is not located
         *        on the device, a temporary buffer will be generated to transfer the data to device,
         *        and an explicit device synchronization will be performed.
         * \tparam SrcBufferRef Type of the source buffer.
         * \tparam DstArrayRef Type of the destination array.
         * \tparam Order Order (dimensions) of the buffers.
         * \param src Source buffer.
         * \param dst Destination array.
         * \param region Subregion of the buffer that will be copied.
         */
        template <typename SrcBufferRef, typename DstArrayRef, uint Order>
        void bufferToArray(const SrcBufferRef& src, DstArrayRef& dst, const CopyRegion<Order>& region)
        {
            // Inferred buffer parameters.

            using SrcBuffer = typename SrcBufferRef::element_type;
            using DstArray = typename DstArrayRef::element_type;
            using SrcValue = typename SrcBuffer::Value;
            using DstValue = typename DstArray::Value;
            using SrcIndexer = typename SrcBuffer::Indexer;
            using SrcLocationTag = typename SrcBuffer::LocationTag;
            constexpr SrcLocationTag srcLocation = SrcBuffer::Location;
            constexpr uint srcOrder = SrcBuffer::Order;
            constexpr uint dstOrder = DstArray::Order;

            // Sanitize inputs.

            static_assert(std::is_same<SrcValue, DstValue>::value,
                "The data type of the given buffers mismatch.");

            static_assert(srcOrder == Order && dstOrder == Order,
                "The order of the given buffers mismatches the input region.");

            HF_ASSERT(all(greaterThanEqual(src->getDims(), region.srcOffset + region.size)),
                "The given copy region is outside of the bounds of the source buffer.");

            HF_ASSERT(all(greaterThanEqual(dst->getDims(), region.dstOffset + region.size)),
                "The given copy region is outside of the bounds of the destination array.");

            // If the buffer is on the device, directly copy from there.

            if (std::is_same<SrcLocationTag, Location::DeviceTag>::value)
            {
                auto dstSurface = Surface<Order, SrcValue>::Create(dst, SurfaceBoundaryMode::Trap);
                Detail::copyRegion(srcLocation, region, src->getConstAccessor(), dstSurface->getAccessor());

                if (HF_SYNCHRONIZE_AT_MEM_TRANSFER)
                {
                    HF_CUDA(DeviceSynchronize());
                }
            }

            // Otherwise, allocate intermediate buffers for transferring from host to array
            // and copy subregions.

            else
            {
                auto srcTemp = Buffer<Location::HostTag, Order, SrcValue, SrcIndexer>::Create(region.size);
                auto dstTemp = Buffer<Location::DeviceTag, Order, SrcValue, SrcIndexer>::Create(region.size);

                const CopyRegion<Order> srcRegion(region.srcOffset, UintN<Order>(0), region.size);
                const CopyRegion<Order> dstRegion(UintN<Order>(0),  region.dstOffset, region.size);

                bufferToBuffer(src, srcTemp, srcRegion);
                bufferToBuffer(srcTemp, dstTemp);
                bufferToArray(dstTemp, dst, dstRegion);
            }
        }

        /**
         * \brief Copies the contents of the given buffer into the array. If the buffer is not located
         *        on the device, a temporary buffer will be generated to transfer the data to device,
         *        and an explicit device synchronization will be performed.
         * \tparam SrcBufferRef Type of the source buffer.
         * \tparam DstArrayRef Type of the destination array.
         * \param src Source buffer.
         * \param dst Destination array.
         */
        template <typename SrcBufferRef, typename DstArrayRef>
        void bufferToArray(const SrcBufferRef& src, DstArrayRef& dst)
        {
            // Inferred buffer parameters.

            using SrcBuffer = typename SrcBufferRef::element_type;
            using DstArray = typename DstArrayRef::element_type;
            using SrcValue = typename SrcBuffer::Value;
            using DstValue = typename DstArray::Value;
            constexpr uint srcOrder = SrcBuffer::Order;
            constexpr uint dstOrder = DstArray::Order;

            // Sanitize inputs.

            static_assert(std::is_same<SrcValue, DstValue>::value,
                "The data type of the given buffers mismatch.");

            static_assert(srcOrder == dstOrder,
                "The order of the given buffers mismatch.");

            HF_ASSERT(src->getDims() == dst->getDims(),
                "Source and destination dimensions mismatch.");

            // Copy whole buffer.

            const CopyRegion<srcOrder> region(src->getDims());
            bufferToArray(src, dst, region);
        }

        //───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───

        /**
         * \brief Copies a subregion of the given array into the buffer. If the buffer is not located
         *        on the device, a temporary buffer will be generated to transfer the data to host,
         *        and an explicit device synchronization will be performed.
         * \tparam SrcArrayRef Type of the source array.
         * \tparam DstBufferRef Type of the destination buffer.
         * \tparam Order Order (dimensions) of the buffers.
         * \param src Source array.
         * \param dst Destination buffer.
         * \param region Subregion of the array that will be copied.
         */
        template <typename SrcArrayRef, typename DstBufferRef, uint Order>
        void arrayToBuffer(const SrcArrayRef& src, DstBufferRef& dst, const CopyRegion<Order>& region)
        {
            // Inferred buffer parameters.

            using SrcArray = typename SrcArrayRef::element_type;
            using DstBuffer = typename DstBufferRef::element_type;
            using SrcValue = typename SrcArray::Value;
            using DstValue = typename DstBuffer::Value;
            using DstIndexer = typename DstBuffer::Indexer;
            using DstLocationTag = typename DstBuffer::LocationTag;
            constexpr DstLocationTag dstLocation = DstBuffer::Location;
            constexpr uint srcOrder = SrcArray::Order;
            constexpr uint dstOrder = DstBuffer::Order;

            // Sanitize inputs.

            static_assert(std::is_same<SrcValue, DstValue>::value,
                "The data type of the given buffers mismatch.");

            static_assert(srcOrder == dstOrder,
                "The order of the given buffers mismatch.");

            HF_ASSERT(all(greaterThanEqual(src->getDims(), region.srcOffset + region.size)),
                "The given copy region is outside of the bounds of the source array.");

            HF_ASSERT(all(greaterThanEqual(dst->getDims(), region.dstOffset + region.size)),
                "The given copy region is outside of the bounds of the destination buffer.");

            // If the buffer is on the device, directly copy from there.

            if (std::is_same<DstLocationTag, Location::DeviceTag>::value)
            {
                auto srcSurface = Surface<Order, SrcValue>::Create(src, SurfaceBoundaryMode::Trap);
                Detail::copyRegion(dstLocation, region, srcSurface->getConstAccessor(), dst->getAccessor());

                if (HF_SYNCHRONIZE_AT_MEM_TRANSFER)
                {
                    HF_CUDA(DeviceSynchronize());
                }
            }

            // Otherwise, allocate intermediate buffers from transferring from host to device.

            else
            {
                auto srcTemp = Buffer<Location::DeviceTag, Order, DstValue, DstIndexer>::Create(region.size);
                auto dstTemp = Buffer<Location::HostTag, Order, DstValue, DstIndexer>::Create(region.size);

                const CopyRegion<Order> srcRegion(region.srcOffset, UintN<Order>(0), region.size);
                const CopyRegion<Order> dstRegion(UintN<Order>(0), region.dstOffset, region.size);

                arrayToBuffer(src, srcTemp, srcRegion);
                bufferToBuffer(srcTemp, dstTemp);
                bufferToBuffer(dstTemp, dst, dstRegion);
            }
        }

        /**
         * \brief Copies the contents of the given array into the buffer. If the buffer is not located
         *        on the device, a temporary buffer will be generated to transfer the data to host,
         *        and an explicit device synchronization will be performed.
         * \tparam SrcArrayRef Type of the source array.
         * \tparam DstBufferRef Type of the destination buffer.
         * \param src Source array.
         * \param dst Destination buffer.
         */
        template <typename SrcArrayRef, typename DstBufferRef>
        void arrayToBuffer(const SrcArrayRef& src, DstBufferRef& dst)
        {
            // Inferred buffer parameters.

            using SrcArray = typename SrcArrayRef::element_type;
            using DstBuffer = typename DstBufferRef::element_type;
            using SrcValue = typename SrcArray::Value;
            using DstValue = typename DstBuffer::Value;
            using DstIndexer = typename DstBuffer::Indexer;
            constexpr uint srcOrder = SrcArray::Order;
            constexpr uint dstOrder = DstBuffer::Order;

            // Sanitize inputs.

            static_assert(std::is_same<SrcValue, DstValue>::value,
                "The data type of the given buffers mismatch.");

            static_assert(srcOrder == dstOrder,
                "The order of the given buffers mismatch.");

            HF_ASSERT(src->getDims() == dst->getDims(),
                "Source and destination dimensions mismatch.");

            // Copy whole array.

            const CopyRegion<srcOrder> region(src->getDims());
            arrayToBuffer(src, dst, region);
        }

        //───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───

        /**
         * \brief Copies a subregion of the given arrays.
         * \tparam SrcArrayRef Type of the source array.
         * \tparam DstArrayRef Type of the destination array.
         * \tparam Order Order of the arrays.
         * \param src Source array.
         * \param dst Destination array.
         * \param region Subregion of the array that will be copied.
         */
        template <typename SrcArrayRef, typename DstArrayRef, uint Order>
        void arrayToArray(const SrcArrayRef& src, DstArrayRef& dst, const CopyRegion<Order>& region)
        {
            // Inferred array parameters.

            using SrcArray = typename SrcArrayRef::element_type;
            using DstArray = typename DstArrayRef::element_type;
            using SrcValue = typename SrcArray::Value;
            using DstValue = typename DstArray::Value;
            constexpr uint srcOrder = SrcArray::Order;
            constexpr uint dstOrder = DstArray::Order;

            // Sanitize inputs.

            static_assert(std::is_same<SrcValue, DstValue>::value,
                "The data type of the given arrays mismatch.");

            static_assert(srcOrder == Order && dstOrder == Order,
                "The order of the given arrays mismatches the input region.");

            HF_ASSERT(all(greaterThanEqual(src->getDims(), region.srcOffset + region.size)),
                "The given copy region is outside of the bounds of the source array.");

            HF_ASSERT(all(greaterThanEqual(dst->getDims(), region.dstOffset + region.size)), 
                "The given copy region is outside of the bounds of the destination buffer.");

            // Copy subregion.

            Detail::copyArrayRegion(src->getArray(), dst->getArray(), region);
        }

        /**
         * \brief Copies the contents of the given arrays.
         * \tparam SrcArrayRef Type of the source array.
         * \tparam DstArrayRef Type of the destination array.
         * \param src Source array.
         * \param dst Destination array.
         */
        template <typename SrcArrayRef, typename DstArrayRef>
        void arrayToArray(const SrcArrayRef& src, DstArrayRef& dst)
        {
            // Inferred array parameters.

            using SrcArray = typename SrcArrayRef::element_type;
            using DstArray = typename DstArrayRef::element_type;
            using SrcValue = typename SrcArray::Value;
            using DstValue = typename DstArray::Value;
            constexpr uint srcOrder = SrcArray::Order;
            constexpr uint dstOrder = DstArray::Order;

            // Sanitize inputs.

            static_assert(std::is_same<SrcValue, DstValue>::value,
                "The data type of the given arrays mismatch.");

            static_assert(srcOrder == dstOrder,
                "The order of the given arrays mismatch.");

            HF_ASSERT(src->getDims() == dst->getDims(), "Source and destination dimensions mismatch.");

            // Copy from one array to the other.

            const CopyRegion<srcOrder> region(src->getDims());
            Detail::copyArrayRegion(src->getArray(), dst->getArray(), region);
        }

        //───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───

        /**
         * \brief Copies (sets) the given value to the buffer.
         * \tparam SrcValue Type of the source value.
         * \tparam DstBufferRef Type of the destination buffer.
         * \param src Source value.
         * \param dst Destination buffer.
         * \param region Subregion of the buffer that will be copied.
         */
        template <typename SrcValue, typename DstBufferRef, uint Order>
        void valueToBuffer(const SrcValue& src, DstBufferRef& dst, const CopyValueRegion<Order>& region)
        {
            // Inferred array parameters.

            using DstBuffer = typename DstBufferRef::element_type;
            using DstValue = typename DstBuffer::Value;
            using DstLocationTag = typename DstBuffer::LocationTag;
            constexpr DstLocationTag dstLocation = DstBuffer::Location;
            constexpr uint dstOrder = DstBuffer::Order;

            // Sanitize inputs.

            static_assert(std::is_same<SrcValue, DstValue>::value,
                "The data type of the given value and buffer mismatch.");

            static_assert(dstOrder == Order,
                "The order of the given buffer mismatch.");

            HF_ASSERT(all(greaterThanEqual(dst->getDims(), region.offset + region.size)), 
                "The given copy region is outside of the bounds of the destination buffer.");

            // Copy subregion.

            Detail::copyValue(dstLocation, region, src, dst->getAccessor());
        }

        /**
         * \brief Copies (sets) the given value to the buffer.
         * \tparam SrcValue Type of the source value.
         * \tparam DstBufferRef Type of the destination buffer.
         * \param src Source value.
         * \param dst Destination buffer.
         */
        template <typename SrcValue, typename DstBufferRef>
        void valueToBuffer(const SrcValue& src, DstBufferRef& dst)
        {
            // Inferred array parameters.

            using DstBuffer = typename DstBufferRef::element_type;
            using DstValue = typename DstBuffer::Value;
            using DstLocationTag = typename DstBuffer::LocationTag;
            constexpr DstLocationTag location = DstBuffer::Location;
            constexpr uint order = DstBuffer::Order;

            // Sanitize inputs.

            static_assert(std::is_same<SrcValue, DstValue>::value,
                "The data type of the given buffer mismatches the input.");

            // Copy from one array to the other.

            const CopyValueRegion<order> region(dst->getDims());
            Detail::copyValue(location, region, src, dst->getAccessor());
        }

        //───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───

        /**
         * \brief Copies (sets) the given value to the array.
         * \tparam SrcValue Type of the source value.
         * \tparam DstArrayRef Type of the destination array.
         * \param src Source value.
         * \param dst Destination array.
         * \param region Subregion of the array that will be copied.
         */
        template <typename SrcValue, typename DstArrayRef, uint Order>
        void valueToArray(const SrcValue& src, DstArrayRef& dst, const CopyValueRegion<Order>& region)
        {
            // Inferred array parameters.

            using DstArray = typename DstArrayRef::element_type;
            using DstValue = typename DstArray::Value;
            constexpr uint dstOrder = DstArray::Order;

            // Sanitize inputs.

            static_assert(std::is_same<SrcValue, DstValue>::value,
                "The data type of the given value and array mismatch.");

            static_assert(dstOrder == Order,
                "The order of the given array mismatches the input region.");

            HF_ASSERT(all(greaterThanEqual(dst->getDims(), region.offset + region.size)),
                "The given copy region is outside of the bounds of the destination buffer.");

            // Copy subregion.

            auto dstSurface = Surface<Order, DstValue>::Create(dst, SurfaceBoundaryMode::Trap);
            Detail::copyValue(Location::Device, region, src, dst->getAccessor());
            
            if (HF_SYNCHRONIZE_AT_MEM_TRANSFER)
            {
                HF_CUDA(DeviceSynchronize());
            }
        }

        /**
         * \brief Copies (sets) the given value to the array.
         * \tparam SrcValue Type of the source value.
         * \tparam DstArrayRef Type of the destination array.
         * \param src Source value.
         * \param dst Destination array.
         */
        template <typename SrcValue, typename DstArrayRef>
        void valueToArray(const SrcValue& src, DstArrayRef& dst)
        {
            // Inferred array parameters.

            using DstArray = typename DstArrayRef::element_type;
            using DstValue = typename DstArray::Value;
            constexpr uint order = DstArray::Order;

            // Sanitize inputs.

            static_assert(std::is_same<SrcValue, DstValue>::value,
                "The data type of the given arrays mismatches the input.");

            // Copy from one array to the other.

            const CopyValueRegion<order> region(dst->getDims());
            
            auto dstSurface = Surface<order, DstValue>::Create(dst, SurfaceBoundaryMode::Trap);
            Detail::copyValue(Location::Device, region, src, dstSurface->getAccessor());

            if (HF_SYNCHRONIZE_AT_MEM_TRANSFER)
            {
                HF_CUDA(DeviceSynchronize());
            }
        }

        //───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───   ───
    }
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_COPY_HPP */
