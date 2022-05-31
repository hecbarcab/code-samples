#ifndef HF_SIMULATOR_COMPUTE_BUFFER_HPP
#define HF_SIMULATOR_COMPUTE_BUFFER_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/Location.hpp>
#include <Simulator/Compute/Buffers/BufferAccessor.hpp>
#include <Simulator/Compute/Buffers/BufferFlags.hpp>
#include <Simulator/Compute/Buffers/BufferAddressMode.hpp>
#include <Simulator/Compute/Buffers/BufferCoordMode.hpp>
#include <Simulator/Compute/Buffers/BufferFilterMode.hpp>
#include <Simulator/Compute/Buffers/BufferSampler.hpp>
#include <Simulator/Compute/Buffers/LinearIndexer.hpp>
#include <Simulator/Compute/Buffers/MortonIndexer.hpp>
#include <Simulator/Compute/Buffers/TiledIndexer.hpp>
#include <Simulator/ThirdParty/npy.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename _Location, uint _Order, typename _Value, typename _Indexer>
    class BaseBuffer
    {
    public:
        static constexpr uint Order = _Order;

        using LocationTag = _Location;
        static constexpr LocationTag Location = {};

        using Base          = BaseBuffer;
        using Value         = _Value;
        using Dims          = IntN<_Order>;
        using Index         = IntN<_Order>;
        using Coords        = FloatN<_Order>;
        using Indexer       = _Indexer;
        using Accessor      = BufferAccessor<BaseBuffer>;
        using ConstAccessor = BufferConstAccessor<BaseBuffer>;
        using Sampler       = BufferSampler<BaseBuffer>;

    protected:
        BaseBuffer(const Dims& dims)
            : _dims(dims)
            , _storage(0)
            , _ptr(nullptr)
        {
        }

    public:
        /**
         * \brief 
         * \return 
         */
        bool isValid() const
        {
            return _ptr != nullptr;
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
        uint getStorage() const
        {
            return _storage;
        }
               
        /**
         * \brief 
         * \return 
         */
        const Value* getPtr() const
        {
            return _ptr;
        }
        
        /**
         * \brief 
         * \return 
         */
        Value* getPtr()
        {
            return _ptr;
        }

        /**
         * \brief 
         * \return 
         */
        ConstAccessor getConstAccessor() const
        {
            return ConstAccessor(_indexer, _ptr);
        }
        
        /**
         * \brief 
         * \return 
         */
        Accessor getAccessor()
        {
            return Accessor(_indexer, _ptr);
        }

        /**
         * \brief 
         * \param addressMode 
         * \param filterMode 
         * \param coordMode 
         * \return 
         */
        Sampler getSampler(BufferAddressMode addressMode, BufferFilterMode filterMode, BufferCoordMode coordMode, bool halfTexelOffset)
        {
            // Determine scale and offset. Mind that the BufferSampler natural way to work is by using node coordinates
            // and buffer space coords. This is the reason why these operations are pretty much the inverse of what we do
            // for TextureSampler.

            const auto scale  = (coordMode == BufferCoordMode::BufferSpace) ? Coords(1.0f) : Coords(_dims);
            const auto offset = halfTexelOffset ? Coords(0.0f) : 0.5f / Coords(_dims);

            return Sampler(getConstAccessor(),
                           _dims,
                           scale,
                           offset,
                           addressMode,
                           filterMode);
        }

    protected:
        Dims    _dims;
        uint    _storage;
        Value*  _ptr;
        Indexer _indexer;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename _Location, uint _Order, typename _Value, typename _Indexer>
    class Buffer;

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order, typename _Value, typename _Indexer>
    class Buffer<Location::HostTag, _Order, _Value, _Indexer> : public BaseBuffer<Location::HostTag, _Order, _Value, _Indexer>
    {
    public:
        using Ref           = Ref<Buffer>;
        using Value         = typename BaseBuffer<Location::HostTag, _Order, _Value, _Indexer>::Value;
        using Dims          = typename BaseBuffer<Location::HostTag, _Order, _Value, _Indexer>::Dims;
        using Index         = typename BaseBuffer<Location::HostTag, _Order, _Value, _Indexer>::Index;
        using Indexer       = typename BaseBuffer<Location::HostTag, _Order, _Value, _Indexer>::Indexer;
        using Accessor      = typename BaseBuffer<Location::HostTag, _Order, _Value, _Indexer>::Accessor;
        using ConstAccessor = typename BaseBuffer<Location::HostTag, _Order, _Value, _Indexer>::ConstAccessor;

    protected:
        Buffer(const Dims& dims, BufferFlags flags)
            : BaseBuffer<Location::HostTag, _Order, Value, Indexer>::BaseBuffer(dims)
            , _flags(flags)
        {
            // Initialize and determine storage required by the indexer.

            _storage = _indexer.setup(dims);

            // Acquire host-allocated memory.

            HF_CUDA(MallocHost(&_ptr, sizeof(Value) * _storage, static_cast<unsigned>(flags)));
        }

    public:
        Buffer() = delete;

        ~Buffer()
        {
            // Free host-allocated memory and reset pointer.

            HF_IGNORE_THROW(HF_CUDA(FreeHost(_ptr)));
            _ptr = nullptr;
        }

        HF_COPY_IMPLEMENTATION(Buffer, delete)
        
        HF_MOVE_IMPLEMENTATION(Buffer, default)

    public:
        /**
         * \brief 
         * \return 
         */
        BufferFlags getFlags() const
        {
            return _flags;
        }

        /**
         * \brief
         * \param fileName
         * \return
         */
        bool loadFromFile(const std::string& filename) const
        {
            // TODO.
            return false;
        }

        /**
         * \brief
         * \param fileName
         * \return
         */
        bool saveToFile(const std::string& filename) const
        {
            // Determine total amount of elements in the buffer.

            const ulong count = compMul(_dims);

            // Copy shape into intermediate buffer.

            std::vector<ulong> shape;
            shape.reserve(_Order);

            for (ulong i = _Order; i > 0; --i)
                shape.push_back(_dims[i - 1]);

            // Copy buffer contents into intermediate buffer.

            std::vector<_Value> data;
            data.reserve(count);
            std::copy(_ptr, _ptr + count, std::back_inserter<std::vector<_Value>>(data));

            // Write buffer into Numpy format

            npy::SaveArrayAsNumpy(filename, false, _Order, shape.data(), data);
            return true;
        }


    protected:
        BufferFlags _flags;


    public:
        static Ref Create(const Dims& dims, BufferFlags flags = BufferFlags::Default)
        {
            return Ref(new Buffer(dims, flags));
        }

        static Ref CreateWithParametersFrom(const Ref& other, const Dims& dims)
        {
            return Create(dims, other->_flags);
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order, typename _Value, typename _Indexer>
    class Buffer<Location::DeviceTag, _Order, _Value, _Indexer> : public BaseBuffer<Location::DeviceTag, _Order, _Value, _Indexer>
    {
    public:
        using Ref           = Ref<Buffer>;
        using Value         = typename BaseBuffer<Location::DeviceTag, _Order, _Value, _Indexer>::Value;
        using Dims          = typename BaseBuffer<Location::DeviceTag, _Order, _Value, _Indexer>::Dims;
        using Index         = typename BaseBuffer<Location::DeviceTag, _Order, _Value, _Indexer>::Index;
        using Indexer       = typename BaseBuffer<Location::DeviceTag, _Order, _Value, _Indexer>::Indexer;
        using Accessor      = typename BaseBuffer<Location::DeviceTag, _Order, _Value, _Indexer>::Accessor;
        using ConstAccessor = typename BaseBuffer<Location::DeviceTag, _Order, _Value, _Indexer>::ConstAccessor;

    protected:
        Buffer(const Dims& dims)
            : BaseBuffer<Location::DeviceTag, _Order, Value, Indexer>::BaseBuffer(dims)
        {
            // Initialize and determine storage required by the indexer.

            _storage = _indexer.setup(dims);

            // Acquire device-allocated memory.

            HF_CUDA(Malloc(&_ptr, sizeof(Value) * _storage));
        }

    public:
        Buffer() = delete;

        ~Buffer()
        {
            // Free device-allocated memory and reset pointer.

            HF_IGNORE_THROW(HF_CUDA(Free(_ptr)));
            _ptr = nullptr;
        }

        HF_COPY_IMPLEMENTATION(Buffer, delete)
        
        HF_MOVE_IMPLEMENTATION(Buffer, default)

    public:
        static Ref Create(const Dims& dims)
        {
            return Ref(new Buffer(dims));
        }

        static Ref CreateWithParametersFrom(const Ref& other, const Dims& dims)
        {
            return Create(dims);
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename L, uint O, typename T, typename I> using HBuffer = typename Buffer<L, O, T, I>::Ref;

    template <uint O, typename T>         using HostBuffer            = Buffer<Location::HostTag, O, T, LinearIndexer<O>>;
    template <uint O, typename T>         using HostMortonBuffer      = Buffer<Location::HostTag, O, T, MortonIndexer<O>>;
    template <uint O, typename T, uint S> using HostTiledBuffer       = Buffer<Location::HostTag, O, T, TiledIndexer<O, S>>;
    template <uint O, typename T>         using DeviceBuffer          = Buffer<Location::DeviceTag, O, T, LinearIndexer<O>>;
    template <uint O, typename T>         using DeviceMortonBuffer    = Buffer<Location::DeviceTag, O, T, MortonIndexer<O>>;
    template <uint O, typename T, uint S> using DeviceTiledBuffer     = Buffer<Location::DeviceTag, O, T, TiledIndexer<O, S>>;
    template <uint O, typename T>         using HostBufferRef         = typename HostBuffer<O, T>::Ref;
    template <uint O, typename T>         using HostMortonBufferRef   = typename HostMortonBuffer<O, T>::Ref;
    template <uint O, typename T, uint S> using HostTiledBufferRef    = typename HostTiledBuffer<O, T, S>::Ref;
    template <uint O, typename T>         using DeviceBufferRef       = typename DeviceBuffer<O, T>::Ref;
    template <uint O, typename T>         using DeviceMortonBufferRef = typename DeviceMortonBuffer<O, T>::Ref;
    template <uint O, typename T, uint S> using DeviceTiledBufferRef  = typename DeviceTiledBuffer<O, T, S>::Ref;

    template <typename T>         using HostBuffer1          = HostBuffer<1, T>;
    template <typename T>         using HostBuffer2          = HostBuffer<2, T>;
    template <typename T>         using HostBuffer3          = HostBuffer<3, T>;
    template <typename T>         using HostMortonBuffer1    = HostMortonBuffer<1, T>;
    template <typename T>         using HostMortonBuffer2    = HostMortonBuffer<2, T>;
    template <typename T>         using HostMortonBuffer3    = HostMortonBuffer<3, T>;
    template <typename T, uint S> using HostTiledBuffer1     = HostTiledBuffer<1, T, S>;
    template <typename T, uint S> using HostTiledBuffer2     = HostTiledBuffer<2, T, S>;
    template <typename T, uint S> using HostTiledBuffer3     = HostTiledBuffer<3, T, S>;
    template <typename T>         using HostBuffer1Ref       = typename HostBuffer1<T>::Ref; 
    template <typename T>         using HostBuffer2Ref       = typename HostBuffer2<T>::Ref;
    template <typename T>         using HostBuffer3Ref       = typename HostBuffer3<T>::Ref;
    template <typename T>         using HostMortonBuffer1Ref = typename HostMortonBuffer1<T>::Ref;
    template <typename T>         using HostMortonBuffer2Ref = typename HostMortonBuffer2<T>::Ref;
    template <typename T>         using HostMortonBuffer3Ref = typename HostMortonBuffer3<T>::Ref;
    template <typename T, uint S> using HostTiledBuffer1Ref  = typename HostTiledBuffer1<T, S>::Ref;
    template <typename T, uint S> using HostTiledBuffer2Ref  = typename HostTiledBuffer2<T, S>::Ref;
    template <typename T, uint S> using HostTiledBuffer3Ref  = typename HostTiledBuffer3<T, S>::Ref;
                                                             
    template <typename T>         using DeviceBuffer1          = DeviceBuffer<1, T>;
    template <typename T>         using DeviceBuffer2          = DeviceBuffer<2, T>;
    template <typename T>         using DeviceBuffer3          = DeviceBuffer<3, T>;
    template <typename T>         using DeviceMortonBuffer1    = DeviceMortonBuffer<1, T>;
    template <typename T>         using DeviceMortonBuffer2    = DeviceMortonBuffer<2, T>;
    template <typename T>         using DeviceMortonBuffer3    = DeviceMortonBuffer<3, T>;
    template <typename T, uint S> using DeviceTiledBuffer1     = DeviceTiledBuffer<1, T, S>;
    template <typename T, uint S> using DeviceTiledBuffer2     = DeviceTiledBuffer<2, T, S>;
    template <typename T, uint S> using DeviceTiledBuffer3     = DeviceTiledBuffer<3, T, S>;
    template <typename T>         using DeviceBuffer1Ref       = typename DeviceBuffer1<T>::Ref; 
    template <typename T>         using DeviceBuffer2Ref       = typename DeviceBuffer2<T>::Ref;
    template <typename T>         using DeviceBuffer3Ref       = typename DeviceBuffer3<T>::Ref;
    template <typename T>         using DeviceMortonBuffer1Ref = typename DeviceMortonBuffer1<T>::Ref;
    template <typename T>         using DeviceMortonBuffer2Ref = typename DeviceMortonBuffer2<T>::Ref;
    template <typename T>         using DeviceMortonBuffer3Ref = typename DeviceMortonBuffer3<T>::Ref;
    template <typename T, uint S> using DeviceTiledBuffer1Ref  = typename DeviceTiledBuffer1<T, S>::Ref;
    template <typename T, uint S> using DeviceTiledBuffer2Ref  = typename DeviceTiledBuffer2<T, S>::Ref;
    template <typename T, uint S> using DeviceTiledBuffer3Ref  = typename DeviceTiledBuffer3<T, S>::Ref;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_BUFFER_HPP */
