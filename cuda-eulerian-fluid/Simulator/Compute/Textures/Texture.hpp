#ifndef HF_SIMULATOR_COMPUTE_TEXTURE_HPP
#define HF_SIMULATOR_COMPUTE_TEXTURE_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/Location.hpp>
#include <Simulator/Compute/Arrays/Array.hpp>
#include <Simulator/Compute/Textures/TextureSampler.hpp>
#include <Simulator/Compute/Textures/TextureResourceType.hpp>
#include <Simulator/Compute/Textures/TextureAddressMode.hpp>
#include <Simulator/Compute/Textures/TextureFilterMode.hpp>
#include <Simulator/Compute/Textures/TextureReadMode.hpp>
#include <Simulator/Compute/Textures/TextureCoordMode.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order, typename _Value>
    class Texture
    {
    public:
        static constexpr uint Order = _Order;
        
        using LocationTag = Location::DeviceTag;
        static constexpr LocationTag Location = Location::Device;

        using Ref          = Ref<Texture>;
        using Value        = _Value;
        using BuiltInValue = typename Traits<_Value>::BuiltInType;
        using Dims         = IntN<Order>;
        using Index        = IntN<Order>;
        using Coords       = FloatN<Order>;
        using Sampler      = TextureSampler<Texture>;

    protected:
        Texture(const ArrayRef<Order, Value>& array, TextureAddressMode addressMode,
                TextureFilterMode filterMode, TextureReadMode readMode,
                TextureCoordMode coordMode, bool srgb, bool halfTexelOffset)
            : _dims(array->getDims())
            , _resource(array->getArray())
            , _resourceType(TextureResourceType::Array)
            , _addressMode(addressMode)
            , _filterMode(filterMode)
            , _readMode(readMode)
            , _srgb(srgb)
            , _halfTexelOffset(halfTexelOffset)
            , _coordMode(coordMode)
        {
            // Allocate resource descriptor pointing to the given array.

            cudaResourceDesc resDesc;
            memset(&resDesc, 0, sizeof(cudaResourceDesc));
            resDesc.resType         = static_cast<cudaResourceType>(TextureResourceType::Array);
            resDesc.res.array.array = array->getArray();

            // Allocate texture descriptor.

            cudaTextureDesc texDesc;
            memset(&texDesc, 0, sizeof(cudaTextureDesc));
            texDesc.addressMode[0]   = static_cast<cudaTextureAddressMode>(addressMode);
            texDesc.addressMode[1]   = static_cast<cudaTextureAddressMode>(addressMode);
            texDesc.addressMode[2]   = static_cast<cudaTextureAddressMode>(addressMode);
            texDesc.filterMode       = static_cast<cudaTextureFilterMode>(filterMode);
            texDesc.readMode         = static_cast<cudaTextureReadMode>(readMode);
            texDesc.sRGB             = srgb ? 1 : 0;
            texDesc.normalizedCoords = 1;

            // Create texture object.

            HF_CUDA(CreateTextureObject(&_texture, &resDesc, &texDesc, nullptr));

            // Determine scale and offset

            _scale  = (coordMode == TextureCoordMode::TextureSpace) ? 1.0f / Coords(_dims) : Coords(1.0f);
            _offset = halfTexelOffset ? 0.5f / Coords(_dims) : Coords(0.0f);
        }


    public:
        Texture() = delete;

        ~Texture()
        {
            HF_HALT_ON_THROW(HF_CUDA(DestroyTextureObject(_texture)));
            _texture = 0;
        }
        
        HF_COPY_IMPLEMENTATION(Texture, delete)
        
        HF_MOVE_IMPLEMENTATION(Texture, default)

    public:
        static Ref Create(const ArrayRef<Order, Value>& array, TextureAddressMode addressMode,
                          TextureFilterMode filterMode, TextureReadMode readMode,
                          TextureCoordMode coordMode, bool srgb, bool halfTexelOffset)
        {
            return Ref(new Texture(array, addressMode, filterMode, readMode, coordMode, srgb, halfTexelOffset));
        }

    public:

        /**
         * \brief 
         * \return 
         */
        bool isValid() const
        {
            return _texture != 0;
        }

        /**
         * \brief
         * \return
         */
        Dims getDims() const
        {
            return _dims;
        }

        /**
         * \brief 
         * \return 
         */
        void* getResource() const
        {
            return _resource;
        }

        /**
         * \brief 
         * \return 
         */
        TextureResourceType getResourceType() const
        {
            return _resourceType;
        }

        /**
         * \brief 
         * \return 
         */
        TextureAddressMode getAddressMode() const
        {
            return _addressMode;
        }

        /**
         * \brief
         * \return
         */
        TextureReadMode getReadMode() const
        {
            return _readMode;
        }

        /**
         * \brief 
         * \return 
         */
        TextureFilterMode getFilterMode() const
        {
            return _filterMode;
        }
        
        /**
         * \brief 
         * \return 
         */
        bool isSRGB() const
        {
            return _srgb;
        }

        /**
         * \brief
         * \return
         */
        bool usesHalfTexelOffset() const
        {
            return _halfTexelOffset;
        }

        /**
         * \brief
         * \return
         */
        TextureCoordMode getCoordMode() const
        {
            return _coordMode;
        }

        /**
         * \brief 
         * \return 
         */
        cudaTextureObject_t getTextureObject() const
        {
            return _texture;
        }

        /**
         * \brief 
         * \return 
         */
        Sampler getSampler() const
        {
            return Sampler(_texture, _scale, _offset);
        }

    private:
        Dims                _dims;
        void*               _resource;
        TextureResourceType _resourceType;
        TextureAddressMode  _addressMode;
        TextureFilterMode   _filterMode;
        TextureReadMode     _readMode;
        bool                _srgb;
        bool                _halfTexelOffset;
        TextureCoordMode    _coordMode;
        cudaTextureObject_t _texture;
        Coords              _scale;
        Coords              _offset;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint O, typename T> using TexturePtr = typename Texture<O, T>::Ref;

    template <typename T> using Texture1    = Texture<1, T>;
    template <typename T> using Texture2    = Texture<2, T>;
    template <typename T> using Texture3    = Texture<3, T>;
    template <typename T> using Texture1Ref = typename Texture1<T>::Ref;
    template <typename T> using Texture2Ref = typename Texture2<T>::Ref;
    template <typename T> using Texture3Ref = typename Texture3<T>::Ref;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_ARRAY_HPP */

