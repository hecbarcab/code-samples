#ifndef HF_SIMULATOR_COMPUTE_SURFACE_ACCESSOR_HPP
#define HF_SIMULATOR_COMPUTE_SURFACE_ACCESSOR_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/Surfaces/SurfaceBoundaryMode.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    namespace Detail
    {
        /**
         * \brief 
         * \tparam Value 
         * \param surface 
         * \param idx 
         * \param boundary
         * \return 
         */
        template <typename Value>
        HF_DINLINE static Value readSurface(cudaSurfaceObject_t surface, const Int1& idx,
                                            cudaSurfaceBoundaryMode boundary)
        {
            return surf1Dread<Value>(surface, sizeof(Value) * idx[0],
                                     boundary);
        }

        /**
         * \brief 
         * \tparam Value 
         * \param surface 
         * \param idx 
         * \param boundaryMode 
         * \return 
         */
        template <typename Value>
        HF_DINLINE static Value readSurface(cudaSurfaceObject_t surface, const Int2& idx,
                                            cudaSurfaceBoundaryMode boundary)
        {
            return surf2Dread<Value>(surface, sizeof(Value) * idx[0], idx[1],
                                     boundary);
        }

        /**
         * \brief 
         * \tparam Value 
         * \param surface 
         * \param idx 
         * \param boundaryMode 
         * \return 
         */
        template <typename Value>
        HF_DINLINE static Value readSurface(cudaSurfaceObject_t surface, const Int3& idx,
                                            cudaSurfaceBoundaryMode boundary)
        {
            return surf3Dread<Value>(surface, sizeof(Value) * idx[0], idx[1], idx[2],
                                     boundary);
        }

        /**
         * \brief 
         * \tparam Value 
         * \param surface 
         * \param idx 
         * \param boundary
         * \return 
         */
        template <typename Value>
        HF_DINLINE static void writeSurface(const Value& value, cudaSurfaceObject_t surface,
                                            const Int1& idx, cudaSurfaceBoundaryMode boundary)
        {
            surf1Dwrite(value, surface, sizeof(Value) * idx[0],
                        boundary);
        }

        /**
         * \brief 
         * \tparam Value 
         * \param surface 
         * \param idx 
         * \param boundary 
         * \return 
         */
        template <typename Value>
        HF_DINLINE static void writeSurface(const Value& value, cudaSurfaceObject_t surface,
                                            const Int2& idx, cudaSurfaceBoundaryMode boundary)
        {
            surf2Dwrite(value, surface, sizeof(Value) * idx[0], idx[1],
                        boundary);
        }

        /**
         * \brief 
         * \tparam Value 
         * \param surface 
         * \param idx 
         * \param boundary
         * \return 
         */
        template <typename Value>
        HF_DINLINE static void writeSurface(const Value& value, cudaSurfaceObject_t surface,
                                            const Int3& idx, cudaSurfaceBoundaryMode boundary)
        {
            surf3Dwrite(value, surface, sizeof(Value) * idx[0], idx[1], idx[2],
                        boundary);
        }
    }

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename _Surface>
    class SurfaceAccessor
    {
    public:
        static constexpr uint Order = _Surface::Order;

        using Surface      = _Surface;
        using Value        = typename Surface::Value;
        using BuiltInValue = typename Surface::BuiltInValue;
        using Index        = typename Surface::Index;

    public:
        SurfaceAccessor() = default;

        SurfaceAccessor(cudaSurfaceObject_t surface, SurfaceBoundaryMode boundaryMode)
            : _surface(surface)
            , _boundaryMode(boundaryMode)
        {
        }

    public:
        HF_DINLINE SurfaceBoundaryMode getBoundaryMode() const
        {
            return _boundaryMode;
        }

        HF_DINLINE Value getValue(const Index& idx) const
        {
            const BuiltInValue value = Detail::readSurface<BuiltInValue>(_surface, idx,
                static_cast<cudaSurfaceBoundaryMode>(_boundaryMode));

            return cast(value);
        }

        HF_DINLINE void setValue(const Index& idx, const Value& value)
        {
            const BuiltInValue builtInValue = cast(value);

            Detail::writeSurface<BuiltInValue>(builtInValue, _surface, idx,
                static_cast<cudaSurfaceBoundaryMode>(_boundaryMode));
        }

    protected:
        cudaSurfaceObject_t _surface;
        SurfaceBoundaryMode _boundaryMode;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename _Surface>
    class SurfaceConstAccessor
    {
    public:
        static constexpr uint Order = _Surface::Order;

        using Surface      = _Surface;
        using Value        = typename Surface::Value;
        using BuiltInValue = typename Surface::BuiltInValue;
        using Dims         = typename Surface::Dims;
        using Index        = typename Surface::Index;

    public:
        SurfaceConstAccessor() = default;

        SurfaceConstAccessor(cudaSurfaceObject_t surface, SurfaceBoundaryMode boundaryMode)
            : _surface(surface)
            , _boundaryMode(boundaryMode)
        {
        }

    public:
        HF_DINLINE SurfaceBoundaryMode getBoundaryMode() const
        {
            return _boundaryMode;
        }

        HF_DINLINE Value getValue(const Index& idx) const
        {
            const BuiltInValue value = Detail::readSurface<BuiltInValue>(_surface, idx,
                static_cast<cudaSurfaceBoundaryMode>(_boundaryMode));

            return cast(value);
        }

    protected:
        cudaSurfaceObject_t _surface;
        SurfaceBoundaryMode _boundaryMode;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_SURFACE_ACCESSOR_HPP */

