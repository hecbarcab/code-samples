#ifndef HF_SIMULATOR_COMPUTE_ARRAY_HPP
#define HF_SIMULATOR_COMPUTE_ARRAY_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/Arrays/ArrayFlags.hpp>
#include <Simulator/Compute/Location.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order, typename _Value>
    class Array
    {
        static_assert(_Order >= 1 && _Order <= 3, "Arrays are limited from 1 to 3 dimensions.");

    public:
        static constexpr uint Order = _Order;
        
        using LocationTag = Location::DeviceTag;
        static constexpr LocationTag Location = Location::Device;

        using Ref          = Ref<Array>;
        using Value        = _Value;
        using BuiltInValue = typename Traits<Value>::BuiltInType;
        using Dims         = IntN<_Order>;
        using Index        = IntN<_Order>;

    protected:
        Array(const Dims& dims, ArrayFlags flags)
            : _dims(dims)
            , _flags(flags)
            , _array(nullptr)
            , _resource(nullptr)
        {
            // Determine extents of the array.

            cudaExtent extent;
            extent.width  = dims[0];
            extent.height = Order >= 2 ? dims[1] : 0;
            extent.depth  = Order >= 3 ? dims[2] : 0;

            // Determine channel format.

            cudaChannelFormatDesc desc = cudaCreateChannelDesc<typename Traits<Value>::BuiltInType>();

            // Allocate CUDA array.
            // TODO: Ensure to initialize array to zero value.

            HF_CUDA(Malloc3DArray(&_array, &desc, extent, static_cast<unsigned>(flags)));
        }

        Array(cudaGraphicsResource_t handle, const Dims& dims, uint index, uint level)
            : _dims(dims)
            , _flags(ArrayFlags::Default)
            , _array(nullptr)
            , _resource(nullptr)
        {
            HF_CUDA(GraphicsMapResources(1, &handle));
            HF_CUDA(GraphicsSubResourceGetMappedArray(&_array, handle, index, level));
            _resource = handle;
        }

    public:
        Array() = delete;

        ~Array()
        {
            if (_resource == nullptr)
            {
                HF_IGNORE_THROW(HF_CUDA(FreeArray(_array)));
                _array = nullptr;
            }
            else
            {
                HF_IGNORE_THROW(HF_CUDA(GraphicsUnmapResources(1, &_resource)));
                _resource = nullptr;
            }
        }
        
        HF_COPY_IMPLEMENTATION(Array, delete)
        
        HF_MOVE_IMPLEMENTATION(Array, default)

    public:
        /**
         * \brief 
         * \return 
         */
        bool isValid() const
        {
            return _array != nullptr;
        }

        /**
         * \brief 
         * \return 
         */
        const Dims& getDims() const
        {
            return _dims;
        }

        /**
         * \brief 
         * \return 
         */
        ArrayFlags getFlags() const
        {
            return _flags;
        }

        /**
         * \brief 
         * \return 
         */
        cudaArray_t getArray() const
        {
            return _array;
        }

        /**
         * \brief 
         * \return 
         */
        cudaGraphicsResource_t getResource() const
        {
            return _resource;
        }


    private:
        Dims                   _dims;
        ArrayFlags             _flags;
        cudaArray_t            _array;
        cudaGraphicsResource_t _resource;

    public:
        static Ref Create(const Dims& dims, ArrayFlags flags)
        {
            return Ref(new Array(dims, flags));
        }

        template <typename Resource>
        static Ref Create(const Resource& resource, const Dims& dims,
                          uint index = 0, uint level = 0)
        {
            return Ref(new Array(resource->getResource(), dims, index, level));
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint O, typename T> using ArrayRef = typename Array<O, T>::Ref;

    template <typename T> using Array1 = Array<1, T>;
    template <typename T> using Array2 = Array<2, T>;
    template <typename T> using Array3 = Array<3, T>;

    template <typename T> using Array1Ref = typename Array<1, T>::Ref;
    template <typename T> using Array2Ref = typename Array<2, T>::Ref;
    template <typename T> using Array3Ref = typename Array<3, T>::Ref;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_ARRAY_HPP */

