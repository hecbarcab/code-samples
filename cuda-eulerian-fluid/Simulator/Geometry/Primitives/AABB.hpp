#ifndef HF_SIMULATOR_GEOMETRY_AABB_HPP
#define HF_SIMULATOR_GEOMETRY_AABB_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Geometry)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    class AABB
    {
    public:
        using Coord                 = FloatN<_Order>;
        using HomogeneousCoord      = FloatN<_Order + 1>;
        using TransformMatrix       = FloatNM<_Order + 1>;
        static constexpr uint Order = _Order;
    
    public:
        AABB()
            : _minPoint(0)
            , _maxPoint(0)
        {
        }

        AABB(const Coord& center, float radius)
            : _minPoint(center - Coord(radius))
            , _maxPoint(center + Coord(radius))
        {
        }

        AABB(const Coord& point1, const Coord& point2)
            : _minPoint(min(point1, point2))
            , _maxPoint(max(point1, point2))
        {
        }

    public:
        HF_HDINLINE bool isNull() const
        {
            return _minPoint == _maxPoint;
        }
                                            
        HF_HDINLINE Coord& getMinPoint()
        {
            return _minPoint;
        }

        HF_HDINLINE const Coord& getMinPoint() const
        {
            return _minPoint;
        }
                                                                 
        HF_HDINLINE Coord& getMaxPoint()
        {
            return _maxPoint;
        }

        HF_HDINLINE const Coord& getMaxPoint() const
        {
            return _maxPoint;
        }

        HF_HDINLINE Coord getDiagonal() const
        {
            return isNull() ? Coord(0) : _maxPoint - _minPoint;
        }

        HF_HDINLINE float getLongestEdge() const
        {
            return compMax(getDiagonal());
        }
        
        HF_HDINLINE float getShortestEdge() const
        {
            return compMin(getDiagonal());
        }
        
        HF_HDINLINE Coord getCenter() const
        {
            return isNull() ? Coord(0) : 0.5f * (_minPoint + _maxPoint);
        }

        HF_HDINLINE Coord getVertex(uint index) const
        {
            Coord vtx;

            for (uint i = 0; i < index; ++i)
                vtx[i] = (index >> i) & 1 == 0 ? _minPoint[i] : _maxPoint[i];

            return vtx;
        }

        HF_HDINLINE constexpr uint getNumVertices() const
        {
            return 1 << (1 + Order);
        }

    public:
        HF_HDINLINE void extend(float Value)
        {
            if (isNull()) return;
            _minPoint -= Coord(Value);
            _maxPoint += Coord(Value);
        }

        HF_HDINLINE void extend(const Coord& point)
        {
            if (!isNull())
            {
                _minPoint = min(point, _minPoint);
                _maxPoint = max(point, _maxPoint);
            }
            else
            {
                _minPoint = point;
                _maxPoint = point;
            }
        }

        HF_HDINLINE void extend(const Coord& point, float radius)
        {
            Coord rad(radius);

            if (!isNull())
            {
                _minPoint = min(point - rad, _minPoint);
                _maxPoint = max(point + rad, _maxPoint);
            }
            else
            {
                _minPoint = point - rad;
                _maxPoint = point + rad;
            }
        }

        HF_HDINLINE void extend(const AABB& other)
        {
            if (isNull())
            {
                _minPoint = other._minPoint;
                _maxPoint = other._maxPoint;
            }
            else
            {
                extend(other._minPoint);
                extend(other._maxPoint);
            }
        }

        HF_HDINLINE void translate(const Coord& amount)
        {
            if (!isNull())
            {
                _minPoint += amount;
                _maxPoint += amount;
            }
        }

        HF_HDINLINE void scale(const Coord& scaling, const Coord& origin)
        {
            if (!isNull())
            {
                _minPoint -= origin;
                _maxPoint -= origin;

                _minPoint *= scaling;
                _maxPoint *= scaling;

                _minPoint += origin;
                _maxPoint += origin;
            }
        }

        HF_HDINLINE void transform(const TransformMatrix& matrix)
        {
            Coord minPoint = Coord(1e10);
            Coord maxPoint = Coord(1e-10);
            Coord point;

            for (uint i = 0; i < (1 << Order); ++i)
            {
                for (uint j = 0; j < Order; ++j)
                    point[j] = (i >> j) % 2 ? _minPoint[j] : _maxPoint[j];
                
                point = Coord(matrix * HomogeneousCoord(point, 1.0f));
                minPoint = min(minPoint, point);
                maxPoint = max(maxPoint, point);
            }

            _minPoint = minPoint;
            _maxPoint = maxPoint;
        }

        HF_HDINLINE bool overlaps(const AABB& other) const
        {
            if (isNull() || other.isNull())
                return false;

            return all(other._minPoint < _maxPoint) 
                && all(other._maxPoint > _minPoint);
        }

        HF_HDINLINE bool contains(const Coord& point) const
        {
            return all(greaterThanEqual(point, _minPoint))
                && all(lessThanEqual(point, _maxPoint));
        }

        HF_HDINLINE float computeSignedDistance(const Coord& point) const
        {
            const Coord center = getCenter();
            const Coord extents = 0.5f * getDiagonal();

            // Source: https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
            const Coord distPerAxis = abs(point - center) - extents;
            return length(max(distPerAxis, 0.0f)) + min(compMax(distPerAxis), 0.0f);
        }

    private:
        Coord _minPoint;
        Coord _maxPoint;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using AABB1 = AABB<1>;
    using AABB2 = AABB<2>;
    using AABB3 = AABB<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Geometry)

#endif /* HF_SIMULATOR_GEOMETRY_AABB_HPP */
