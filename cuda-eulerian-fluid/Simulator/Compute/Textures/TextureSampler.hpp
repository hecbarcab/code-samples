#ifndef HF_SIMULATOR_COMPUTE_TEXTURE_SAMPLER_HPP
#define HF_SIMULATOR_COMPUTE_TEXTURE_SAMPLER_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Utility/Assertions.hpp>
#include <Simulator/Utility/Macros.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    namespace Detail
    {
        template <typename Value>
        HF_DINLINE static Value sampleTexture(cudaTextureObject_t texture, const Float1& coords)
        {
            return tex1D<Value>(texture, coords[0]);
        }

        template <typename Value>
        HF_DINLINE static Value sampleTexture(cudaTextureObject_t texture, const Float2& coords)
        {
            return tex2D<Value>(texture, coords[0], coords[1]);
        }

        template <typename Value>
        HF_DINLINE static Value sampleTexture(cudaTextureObject_t texture, const Float3& coords)
        {
            return tex3D<Value>(texture, coords[0], coords[1], coords[2]);
        }
    }

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename _Texture>
    class TextureSampler
    {
    public:
        static constexpr uint Order = _Texture::Order;
        using Texture               = _Texture;
        using Value                 = typename Texture::Value;
        using BuiltInValue          = typename Texture::BuiltInValue;
        using Dims                  = IntN<Order>;
        using Coords                = FloatN<Order>;

    public:
        TextureSampler()
            : _scale(1.0f)
            , _offset(0.0f)
            , _texture(0)
        {
        }

        TextureSampler(cudaTextureObject_t texture, const Coords& scale, const Coords& offset)
            : _scale(scale)
            , _offset(offset)
            , _texture(texture)
        {
        }

    public:
        /**
         * \brief
         * \param coords
         * \return
         */
        HF_DINLINE Value getValue(const Coords& coords) const
        {
            const Coords textureCoords = coords * _scale + _offset;
            const BuiltInValue value = Detail::sampleTexture<BuiltInValue>(_texture, textureCoords);
            return cast(value);
        }

    protected:
        Coords              _scale;
        Coords              _offset;
        cudaTextureObject_t _texture;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_TEXTURE_SAMPLER_HPP */

