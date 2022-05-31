#ifndef HF_SIMULATOR_COMPUTE_BUFFER_SAMPLER_HPP
#define HF_SIMULATOR_COMPUTE_BUFFER_SAMPLER_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/Buffers/BufferAddressMode.hpp>
#include <Simulator/Compute/Buffers/BufferFilterMode.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename _Buffer>
    class BaseBufferSampler
    {
    public:
        static constexpr uint Order = _Buffer::Order;
        using Buffer                = _Buffer;
        using ConstAccessor         = typename Buffer::ConstAccessor;
        using Value                 = typename Buffer::Value;
        using Dims                  = IntN<Order>;
        using Index                 = IntN<Order>;
        using Coords                = FloatN<Order>;

    public:
        BaseBufferSampler() = default;

        BaseBufferSampler(const ConstAccessor& accessor,
                          const Dims&          dims,
                          const Coords&        scale,
                          const Coords&        offset,
                          BufferAddressMode    addressMode,
                          BufferFilterMode     filterMode)
            : _accessor(accessor)
            , _dims(dims)
            , _scale(scale)
            , _offset(offset)
            , _addressMode(addressMode)
            , _filterMode(filterMode)
        {
        }

    protected:

        HF_HDINLINE Coords applyCoordinatesMode(const Coords& coords) const
        {
            return coords * _scale + _offset;
        }

        HF_HDINLINE Index applyAddressingMode(const Index& idx) const
        {
            return (_addressMode == BufferAddressMode::Wrap)
                 ? (((idx % _dims) + _dims) % _dims)
                 : clamp(idx, Index(0), _dims - 1);
        }

    protected:
        ConstAccessor     _accessor;
        Dims              _dims;
        Coords            _scale;
        Coords            _offset;
        BufferAddressMode _addressMode;
        BufferFilterMode  _filterMode;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename _Buffer, uint _Order = _Buffer::Order>
    class BufferSampler;

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename _Buffer>
    class BufferSampler<_Buffer, 1> : public BaseBufferSampler<_Buffer>
    {
    public:
        using Buffer        = typename BaseBufferSampler<_Buffer>::Buffer;
        using ConstAccessor = typename BaseBufferSampler<_Buffer>::ConstAccessor;
        using Value         = typename BaseBufferSampler<_Buffer>::Value;
        using Dims          = typename BaseBufferSampler<_Buffer>::Dims;
        using Index         = typename BaseBufferSampler<_Buffer>::Index;
        using Coords        = typename BaseBufferSampler<_Buffer>::Coords;

    public:
        using BaseBufferSampler<_Buffer>::BaseBufferSampler;

    public:
        /**
         * \brief
         * \param coords
         * \return
         */
        HF_HDINLINE Value getValue(const Coords& coords) const
        {
            Coords gridCoords = applyCoordinatesMode(coords);

            if (_filterMode == BufferFilterMode::PointFilter)
            {
                const Index idx = applyAddressingMode(Index(round(gridCoords)));
                return _accessor.getValue(idx);
            }
            else
            {
                const Index minIdx = applyAddressingMode(Index(floor(gridCoords)));
                const Index maxIdx = applyAddressingMode(Index(ceil(gridCoords)));

                const Value value0 = _accessor.getValue(minIdx);
                const Value value1 = _accessor.getValue(maxIdx);

                const Coords interp = glm::fract(gridCoords);
                return mix(value0, value1, interp.x);
            }
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename _Buffer>
    class BufferSampler<_Buffer, 2> : public BaseBufferSampler<_Buffer>
    {
    public:
        using Buffer        = typename BaseBufferSampler<_Buffer>::Buffer;
        using ConstAccessor = typename BaseBufferSampler<_Buffer>::ConstAccessor;
        using Value         = typename BaseBufferSampler<_Buffer>::Value;
        using Dims          = typename BaseBufferSampler<_Buffer>::Dims;
        using Index         = typename BaseBufferSampler<_Buffer>::Index;
        using Coords        = typename BaseBufferSampler<_Buffer>::Coords;

    public:
        using BaseBufferSampler<_Buffer>::BaseBufferSampler;

    public:
        /**
         * \brief
         * \param coords
         * \return
         */
        HF_HDINLINE Value getValue(const Coords& coords) const
        {
            Coords gridCoords = applyCoordinatesMode(coords);

            if (_filterMode == BufferFilterMode::PointFilter)
            {
                const Index idx = applyAddressingMode(Index(round(gridCoords)));
                return _accessor.getValue(idx);
            }
            else
            {
                const Index minIdx = applyAddressingMode(Index(floor(gridCoords)));
                const Index maxIdx = applyAddressingMode(Index(ceil(gridCoords)));

                const Value value00 = _accessor.getValue({ minIdx.x, minIdx.y });
                const Value value10 = _accessor.getValue({ maxIdx.x, minIdx.y });
                const Value value01 = _accessor.getValue({ minIdx.x, maxIdx.y });
                const Value value11 = _accessor.getValue({ maxIdx.x, maxIdx.y });

                const Coords interp = glm::fract(gridCoords);
                const Value value0 = mix(value00, value10, interp.x);
                const Value value1 = mix(value01, value11, interp.x);
                return mix(value0, value1, interp.y);
            }
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename _Buffer>
    class BufferSampler<_Buffer, 3> : public BaseBufferSampler<_Buffer>
    {
        friend _Buffer;

    public:
        using Buffer        = typename BaseBufferSampler<_Buffer>::Buffer;
        using ConstAccessor = typename BaseBufferSampler<_Buffer>::ConstAccessor;
        using Value         = typename BaseBufferSampler<_Buffer>::Value;
        using Dims          = typename BaseBufferSampler<_Buffer>::Dims;
        using Index         = typename BaseBufferSampler<_Buffer>::Index;
        using Coords        = typename BaseBufferSampler<_Buffer>::Coords;

    public:
        using BaseBufferSampler<_Buffer>::BaseBufferSampler;

    public:
        /**
         * \brief
         * \param coords
         * \return
         */
        HF_HDINLINE Value getValue(const Coords& coords) const
        {
            Coords gridCoords = applyCoordinatesMode(coords);

            if (_filterMode == BufferFilterMode::PointFilter)
            {
                const Index idx = applyAddressingMode(Index(round(gridCoords)));
                return _accessor.getValue(idx);
            }
            else
            {
                const Index minIdx = applyAddressingMode(Index(floor(gridCoords)));
                const Index maxIdx = applyAddressingMode(Index(ceil(gridCoords)));

                const Value& value000 = _accessor.getValue({ minIdx.x, minIdx.y, minIdx.z });
                const Value& value100 = _accessor.getValue({ maxIdx.x, minIdx.y, minIdx.z });
                const Value& value010 = _accessor.getValue({ minIdx.x, maxIdx.y, minIdx.z });
                const Value& value110 = _accessor.getValue({ maxIdx.x, maxIdx.y, minIdx.z });
                const Value& value001 = _accessor.getValue({ minIdx.x, minIdx.y, maxIdx.z });
                const Value& value101 = _accessor.getValue({ maxIdx.x, minIdx.y, maxIdx.z });
                const Value& value011 = _accessor.getValue({ minIdx.x, maxIdx.y, maxIdx.z });
                const Value& value111 = _accessor.getValue({ maxIdx.x, maxIdx.y, maxIdx.z });

                const Coords interp = fract(gridCoords);
                const Value value00 = mix(value000, value100, interp.x);
                const Value value10 = mix(value010, value110, interp.x);
                const Value value01 = mix(value001, value101, interp.x);
                const Value value11 = mix(value011, value111, interp.x);
                const Value value0 = mix(value00, value10, interp.y);
                const Value value1 = mix(value01, value11, interp.y);
                return mix(value0, value1, interp.z);
            }
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_TEXTURE_SAMPLER_HPP */

