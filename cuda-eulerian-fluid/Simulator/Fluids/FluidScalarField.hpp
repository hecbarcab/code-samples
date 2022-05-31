#ifndef HF_SIMULATOR_FLUID_SCALAR_FIELD_HPP
#define HF_SIMULATOR_FLUID_SCALAR_FIELD_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/Buffers/Buffer.hpp>
#include <Simulator/Compute/Arrays/Array.hpp>
#include <Simulator/Compute/Surfaces/Surface.hpp>
#include <Simulator/Compute/Textures/Texture.hpp>
#include <Simulator/Compute/Copy.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order, typename _Value>
    class FluidScalarField
    {
    public:
        static constexpr uint Order = _Order;

        // Basic information regarding the grid

        using Ref                 = Ref<FluidScalarField>;
        using Value               = _Value;
        using Domain              = FluidDomain<_Order>;
        using Dims                = IntN<_Order>;
        using Index               = IntN<_Order>;
        using CopyRegion          = Compute::CopyRegion<_Order>;

        // Host-side datatypes

        using HostBuffer          = Compute::HostBuffer<Order, Value>;
        using HostBufferRef       = typename HostBuffer::Ref;
        using HostAccessor        = typename HostBuffer::Accessor;
        using HostConstAccessor   = typename HostBuffer::ConstAccessor;
        using HostSampler         = typename HostBuffer::Sampler;

        // Device-side datatypes

        using DeviceArray         = Compute::Array<Order, Value>;
        #if HF_DEVICE_FIELDS_AS_ARRAYS == true
        using DeviceArrayRef      = typename DeviceArray::Ref;
        using DeviceSurface       = Compute::Surface<Order, Value>;
        using DeviceSurfaceRef    = typename DeviceSurface::Ref;
        using DeviceTexture       = Compute::Texture<Order, Value>;
        using DeviceTexturePtr    = typename DeviceTexture::Ref;
        using DeviceAccessor      = typename DeviceSurface::Accessor;
        using DeviceConstAccessor = typename DeviceSurface::ConstAccessor;
        using DeviceSampler       = typename DeviceTexture::Sampler;
        #else
        using DeviceBuffer        = Compute::HostBuffer<Order, Value>;
        using DeviceBufferRef     = typename DeviceBuffer::Ref;
        using DeviceAccessor      = typename DeviceBuffer::Accessor;
        using DeviceConstAccessor = typename DeviceBuffer::ConstAccessor;
        using DeviceSampler       = typename DeviceBuffer::Sampler;
        #endif
        
    protected:
        FluidScalarField(const Dims& dims)
            : _dims(dims)
        {
            // Allocate host-side buffer.

            _hostBuffer  = HostBuffer::Create(dims, Compute::BufferFlags::Default);

            // Allocate device-side array and corresponding data structures.

            #if HF_DEVICE_FIELDS_AS_ARRAYS == true
            const bool isFloatingPointType = Traits<Value>::IsFloatingPoint;

            _deviceArray = DeviceArray::Create(dims,
                                               Compute::ArrayFlags::SurfaceLoadStore);
            
            _deviceSurface = DeviceSurface::Create(_deviceArray,
                                                   Compute::SurfaceBoundaryMode::Clamp);

            _deviceTexture = DeviceTexture::Create(_deviceArray,
                                                   Compute::TextureAddressMode::Clamp,
                                                   isFloatingPointType ? Compute::TextureFilterMode::LinearFilter 
                                                                       : Compute::TextureFilterMode::PointFilter,
                                                   Compute::TextureReadMode::ElementType,
                                                   Compute::TextureCoordMode::TextureSpace,
                                                   false,
                                                   true);
            #else
            _deviceBuffer = DeviceBuffer::Create(dims);
            #endif
        }

        FluidScalarField(const Domain& domain)
            : FluidScalarField(domain.getDims())
        {
        }

    public:
        const Dims& getDims() const
        {
            return _dims;
        }

        HostConstAccessor getConstAccessor(const Compute::Location::HostTag&) const
        {
            return _hostBuffer->getConstAccessor();
        }

        HostAccessor getAccessor(const Compute::Location::HostTag&)
        {
            return _hostBuffer->getAccessor();
        }

        HostSampler getSampler(const Compute::Location::HostTag&) const
        {
            const bool isFloatingPointType = Traits<Value>::IsFloatingPoint;

            return _hostBuffer->getSampler(Compute::BufferAddressMode::Clamp,
                                           isFloatingPointType ? Compute::BufferFilterMode::LinearFilter
                                                               : Compute::BufferFilterMode::PointFilter,
                                           Compute::BufferCoordMode::BufferSpace,
                                           true);
        }

        DeviceConstAccessor getConstAccessor(const Compute::Location::DeviceTag&) const
        {
            #if HF_DEVICE_FIELDS_AS_ARRAYS == true
            return _deviceSurface->getConstAccessor();
            #else
            return _deviceBuffer->getConstAccessor();
            #endif
        }

        DeviceAccessor getAccessor(const Compute::Location::DeviceTag&)
        {
            #if HF_DEVICE_FIELDS_AS_ARRAYS == true
            return _deviceSurface->getAccessor();
            #else
            return _deviceBuffer->getAccessor();
            #endif
        }

        DeviceSampler getSampler(const Compute::Location::DeviceTag&) const
        {
            #if HF_DEVICE_FIELDS_AS_ARRAYS == true
            return _deviceTexture->getSampler();
            #else
            const bool isFloatingPointType = Traits<Value>::IsFloatingPoint;

            return _deviceBuffer->getSampler(Compute::BufferAddressMode::Clamp,
                                             isFloatingPointType ? Compute::BufferFilterMode::LinearFilter
                                                                 : Compute::BufferFilterMode::PointFilter,
                                             Compute::BufferCoordMode::BufferSpace,
                                             true);
            #endif
        }

        void clear(const Compute::Location::HostTag&, const Value& value)
        {
            Compute::Copy::valueToBuffer(value, _hostBuffer);
        }

        void clear(const Compute::Location::DeviceTag&, const Value& value)
        {
            #if HF_DEVICE_FIELDS_AS_ARRAYS == true
            Compute::Copy::valueToArray(value, _deviceArray);
            #else
            Compute::Copy::valueToBuffer(value, _deviceBuffer);
            #endif
        }

        void clear(const Value& value)
        {
            clear(Compute::Location::Host, value);
            clear(Compute::Location::Device, value);
        }

        void copyHostToDevice()
        {
            #if HF_DEVICE_FIELDS_AS_ARRAYS == true
            Compute::Copy::bufferToArray(_hostBuffer, _deviceArray);
            #else
            Compute::Copy::bufferToBuffer(_hostBuffer, _deviceBuffer);
            #endif
        }

        void copyHostToDevice(const CopyRegion& region)
        {
            #if HF_DEVICE_FIELDS_AS_ARRAYS == true
            Compute::Copy::bufferToArray(_hostBuffer, _deviceArray, region);
            #else
            Compute::Copy::bufferToBuffer(_hostBuffer, _deviceBuffer, region);
            #endif
        }

        void copyDeviceToHost()
        {
            #if HF_DEVICE_FIELDS_AS_ARRAYS == true
            Compute::Copy::arrayToBuffer(_deviceArray, _hostBuffer);
            #else
            Compute::Copy::bufferToBuffer(_deviceBuffer, _hostBuffer);
            #endif
        }

        void copyDeviceToHost(const CopyRegion& region)
        {
            #if HF_DEVICE_FIELDS_AS_ARRAYS == true
            Compute::Copy::arrayToBuffer(_deviceArray, _hostBuffer, region);
            #else
            Compute::Copy::bufferToBuffer(_deviceBuffer, _hostBuffer, region);
            #endif
        }

        template <typename Resource>
        void copyDeviceToResource(const Resource& res) const
        {
            auto resourceArray = DeviceArray::Create(res, getDims());

            #if HF_DEVICE_FIELDS_AS_ARRAYS == true
            Compute::Copy::arrayToArray(_deviceArray, resourceArray);
            #else
            Compute::Copy::bufferToArray(_deviceBuffer, resourceArray);
            #endif
        }

        template <typename Resource>
        void copyDeviceToResource(const Resource& res, const CopyRegion& region) const
        {
            auto resourceArray = DeviceArray::Create(res, getDims());

            #if HF_DEVICE_FIELDS_AS_ARRAYS == true
            Compute::Copy::arrayToArray(_deviceArray, resourceArray, region);
            #else
            Compute::Copy::bufferToArray(_deviceBuffer, resourceArray, region);
            #endif
        }

        bool loadFromFile(const Compute::Location::DeviceTag&, const std::string& filename) const
        {
            if (loadFromFile(Compute::Location::Host, filename))
            {
                copyHostToDevice();
                return true;
            }
            
            return false;
        }

        bool loadFromFile(const Compute::Location::HostTag&, const std::string& filename) const
        {
            return _hostBuffer->loadFromFile(filename);
        }

        bool saveToFile(const Compute::Location::DeviceTag&, const std::string& filename)
        {
            copyDeviceToHost();
            return saveToFile(Compute::Location::Host, filename);
        }

        bool saveToFile(const Compute::Location::HostTag&, const std::string& filename) const
        {
            return _hostBuffer->saveToFile(filename);
        }

    public:
        Dims             _dims;
        HostBufferRef    _hostBuffer;
        #if HF_DEVICE_FIELDS_AS_ARRAYS == true
        DeviceArrayRef   _deviceArray;
        DeviceSurfaceRef _deviceSurface;
        DeviceTexturePtr _deviceTexture;
        #else
        DeviceBufferRef  _deviceBuffer;
        #endif

    public:
        static Ref Create(const Domain& domain)
        {
            return Ref(new FluidScalarField(domain));
        }

        static Ref Create(const Dims& dims)
        {
            return Ref(new FluidScalarField(dims));
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint O, typename V> using FluidScalarFieldRef = typename FluidScalarField<O, V>::Ref;

    template <typename V> using FluidScalarField1 = FluidScalarField<1, V>;
    template <typename V> using FluidScalarField2 = FluidScalarField<2, V>;
    template <typename V> using FluidScalarField3 = FluidScalarField<3, V>;
    template <typename V> using FluidScalarField1Ref = FluidScalarFieldRef<1, V>;
    template <typename V> using FluidScalarField2Ref = FluidScalarFieldRef<2, V>;
    template <typename V> using FluidScalarField3Ref = FluidScalarFieldRef<3, V>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUID_SCALAR_FIELD_HPP */
                        