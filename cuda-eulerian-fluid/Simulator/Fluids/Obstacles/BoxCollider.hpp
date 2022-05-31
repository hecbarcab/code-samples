#ifndef HF_SIMULATION_FLUID_BOX_OBSTACLE_HPP
#define HF_SIMULATION_FLUID_BOX_OBSTACLE_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Geometry/Primitives/AABB.hpp>
#include <utility>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    class BoxCollider
    {
    public:
        static constexpr uint Order = _Order;
        using Coord            = FloatN<_Order>;
        using HomogeneousCoord = FloatN<_Order + 1>;
        using TransformMatrix  = FloatNM<Order + 1>;
        using AABB             = Geometry::AABB<_Order>;

    public:
        BoxCollider()
            : _transform(1.0f)
            , _extents(1.0f)
            , _velocity(0.0f)
            , _radius(0.0f)
        {
        }

        BoxCollider(const TransformMatrix& transform, const Coord& extents, const Coord& velocity, float radius)
            : _transform(transform)
            , _extents(extents)
            , _velocity(velocity)
            , _radius(radius)
        {
        }

    public:
        HF_HDINLINE const TransformMatrix& getTransformMatrix() const
        {
            return _transform;
        }

        HF_HDINLINE void setTransformMatrix(const TransformMatrix& transform)
        {
            _transform = transform;
        }

        HF_HDINLINE const Coord& getExtents() const
        {
            return _extents;
        }

        HF_HDINLINE void setExtents(const Coord& extents)
        {
            _extents = extents;
        }

        HF_HDINLINE const Coord& getVelocity() const
        {
            return _velocity;
        }

        HF_HDINLINE void setVelocity(const Coord& velocity)
        {
            _velocity = velocity;
        }

        HF_HDINLINE float getRadius() const
        {
            return _radius;
        }

        HF_HDINLINE void setRadius(float radius)
        {
            _radius = radius;
        }

    public:
        HF_HDINLINE void computeDistanceAndVelocity(const Coord& point, float& dist, Coord& velocity)
        {
            // Source: https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm

            const Coord tformedPoint = Coord(_transform * HomogeneousCoord(point, 1.0f));
            const Coord distPerAxis = abs(tformedPoint) - _extents;
            dist = length(max(distPerAxis, 0.0f)) + min(compMax(distPerAxis), 0.0f) - _radius;
            velocity = _velocity;
        }

        HF_HDINLINE AABB getBoundingBox() const
        {
            AABB aabb(-_extents, _extents);
            aabb.transform(inverse(_transform));
            aabb.extend(_radius);
            return aabb;
        }

    private:
        TransformMatrix _transform;
        Coord           _extents;
        Coord           _velocity;
        float           _radius;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using BoxCollider2 = BoxCollider<2>;
    using BoxCollider3 = BoxCollider<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATION_FLUID_SPHERE_OBSTACLE_HPP */
