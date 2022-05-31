#ifndef HF_SIMULATOR_FLUID_DOMAIN_HPP
#define HF_SIMULATOR_FLUID_DOMAIN_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Geometry/Primitives/AABB.hpp>
#include <Simulator/Fluids/FluidDomainFace.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    class FluidDomain
    {
    public:
        static constexpr uint Order = _Order;
        using Dims   = IntN<_Order>;
        using Index  = IntN<_Order>;
        using Coords = FloatN<_Order>;
        using AABB   = Geometry::AABB<_Order>;

    public:
        FluidDomain(const AABB& region, const Dims& dims)
            : _region(region)
            , _dims(dims)
            , _dx(region.getDiagonal() / Coords(dims))
        {
        }

    public:
        HF_HDINLINE const AABB& getRegion() const
        {
            return _region;
        }

        HF_HDINLINE const Dims& getDims() const
        {
            return _dims;
        }

        HF_HDINLINE const Coords& getDx() const
        {
            return _dx;
        }

        HF_HDINLINE const Coords& getOneOverDx() const
        {
            return 1.0f / _dx;
        }

    public:

        HF_HDINLINE Dims getDimsOfFaceGrid(int axis) const
        {
            Dims dims = _dims;
            dims[axis] += 1;
            return dims;
        }
         
        HF_HDINLINE Dims getDimsOfNodesGrid() const
        {
            return _dims + Dims(1);
        }

        HF_HDINLINE Coords getNodePosition(const Coords& coords) const
        {
            return _region.getMinPoint() + _dx * coords;
        }

        HF_HDINLINE Coords getNodePosition(const Index& idx) const
        {
            return getNodePosition(Coords(idx));
        }

        HF_HDINLINE Coords getCellPosition(const Coords& coords) const
        {
            Coords nodeCoords = coords + 0.5f;
            return getNodePosition(nodeCoords);
        }

        HF_HDINLINE Coords getCellPosition(const Index& idx) const
        {
            return getCellPosition(Coords(idx));
        }

        HF_HDINLINE Coords getFacePosition(const Coords& coords, int axis) const
        {
            Coords nodeCoords(coords);

            for (uint i = 0; i < axis;  ++i)
                nodeCoords[i] += 0.5f;

            for (uint i = axis + 1; i < Order; ++i)
                nodeCoords[i] += 0.5f;

            return getNodePosition(nodeCoords);
        }

        HF_HDINLINE Coords getFacePosition(const Index& idx, int axis) const
        {
            return getFacePosition(Coords(idx), axis);
        }

        HF_HDINLINE Coords getNodeCoords(const Coords& point) const
        {
            return ((point - _region.getMinPoint()) / _dx);
        }

        HF_HDINLINE Coords getCellCoords(const Coords& point) const
        {
            Coords nodeCoords = getNodeCoords(point);
            return nodeCoords - 0.5f;
        }

        HF_HDINLINE Coords getFaceCoords(const Coords& point, int axis) const
        {
            Coords nodeCoords = getNodeCoords(point);

            for (uint i = 0; i < axis;  ++i)
                nodeCoords[i] -= 0.5f;

            for (uint i = axis + 1; i < Order; ++i) 
                nodeCoords[i] -= 0.5f;

            return nodeCoords;
        }

        HF_HDINLINE Index getNodeIndex(const Coords& coords, bool clampToDomain = true) const
        {
            Index index(coords);
            
            if (clampToDomain)
                index = clamp(index, Index(0), _dims);

            return index;
        }

        HF_HDINLINE Index getCellIndex(const Coords& coords, bool clampToDomain = true) const
        {
            Index index(coords);

            if (clampToDomain)
                index = clamp(index, Index(0), _dims - Index(1));

            return index;
        }

        HF_HDINLINE Index getFaceIndex(const Coords& coords, int axis, bool clampToDomain = true) const
        {
            Index index(coords);

            if (clampToDomain)
                index = clamp(index, Index(0), getDimsOfFaceGrid(axis) - Index(1));

            return index;
        }

        HF_HDINLINE FluidDomainFace getDomainFace(const Index& idx) const
        {
            // TODO: Ideally we should return the face closest to the index.

            for (uint i = 0; i < Order; i++)
            {
                if (idx[i] < 0)         return static_cast<FluidDomainFace>(1 + 2 * i);
                if (idx[i] >= _dims[i]) return static_cast<FluidDomainFace>(2 + 2 * i);
            }

            return FluidDomainFace::None;
        }

        HF_HDINLINE Coords getDomainFaceNormal(FluidDomainFace face) const
        {
            Coords normal(0.0f);

            if (face != FluidDomainFace::None)
            {
                const uint idx = static_cast<uint>(face) - 1;
                const uint axis = idx >> 1;
                const uint sign = idx & 1;
                normal[axis] = sign ? 1.0f : -1.0f;
            }

            return normal;
        }

    private:
        AABB   _region;
        Dims   _dims;
        Coords _dx;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using FluidDomain1 = FluidDomain<1>;
    using FluidDomain2 = FluidDomain<2>;
    using FluidDomain3 = FluidDomain<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUID_DOMAIN_HPP */
