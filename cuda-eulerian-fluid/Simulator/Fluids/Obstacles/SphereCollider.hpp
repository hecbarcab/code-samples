#ifndef HF_SIMULATION_FLUID_SPHERE_OBSTACLE_HPP
#define HF_SIMULATION_FLUID_SPHERE_OBSTACLE_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Geometry/Primitives/AABB.hpp>
#include <utility>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    class SphereCollider
    {
    public:
        static constexpr uint Order = _Order;
        using FloatN = FloatN<_Order>;
        using AABB = Geometry::AABB<_Order>;

    public:
        SphereCollider()
            : _center(0.0f)
            , _velocity(0.0f)
            , _radius(10.0f)
        {
        }

        SphereCollider(const FloatN& center, const FloatN& velocity, float radius)
            : _center(center)
            , _velocity(velocity)
            , _radius(radius)
        {
        }

    public:
        HF_HDINLINE const FloatN& getCenter() const
        {
            return _center;
        }

        HF_HDINLINE void setCenter(const FloatN& center)
        {
            _center = center;
        }

        HF_HDINLINE const FloatN& getVelocity() const
        {
            return _velocity;
        }

        HF_HDINLINE void setVelocity(const FloatN& velocity)
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
        HF_HDINLINE void computeDistanceAndVelocity(const FloatN& point, float& dist, FloatN& velocity)
        {
            dist = distance(point, _center) - _radius;
            velocity = _velocity;
        }

        HF_HDINLINE AABB getBoundingBox() const
        {
            const FloatN minPoint = _center - _radius;
            const FloatN maxPoint = _center + _radius;
            return AABB(minPoint, maxPoint);
        }

    private:
        FloatN _center;
        FloatN _velocity;
        float  _radius;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using CircleCollider2 = SphereCollider<2>;
    using SphereCollider3 = SphereCollider<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATION_FLUID_SPHERE_OBSTACLE_HPP */
