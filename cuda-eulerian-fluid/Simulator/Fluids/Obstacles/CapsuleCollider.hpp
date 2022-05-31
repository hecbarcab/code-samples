#ifndef HF_SIMULATION_FLUID_CAPSULE_COLLIDER_HPP
#define HF_SIMULATION_FLUID_CAPSULE_COLLIDER_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Geometry/Primitives/AABB.hpp>
#include <utility>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    class CapsuleCollider
    {
    public:
        static constexpr uint Order = _Order;
        using FloatN = FloatN<_Order>;
        using AABB = Geometry::AABB<_Order>;

    public:
        CapsuleCollider()
            : _start(0.0f)
            , _end(0.0f)
            , _startVelocity(0.0f)
            , _endVelocity(0.0f)
            , _radius(10.0f)
        {
        }

        CapsuleCollider(const FloatN& start, const FloatN& end, const FloatN& startVelocity, const FloatN& endVelocity, float radius)
            : _start(start)
            , _end(end)
            , _startVelocity(startVelocity)
            , _endVelocity(endVelocity)
            , _radius(radius)
        {
        }

    public:
        HF_HDINLINE const FloatN& getStart() const
        {
            return _start;
        }

        HF_HDINLINE const FloatN& getStartVelocity() const
        {
            return _startVelocity;
        }

        HF_HDINLINE void setStart(const FloatN& start, const FloatN& startVelocity)
        {
            _start = start;
            _startVelocity = startVelocity;
        }

        HF_HDINLINE const FloatN& getEnd() const
        {
            return _end;
        }

        HF_HDINLINE const FloatN& getEndVelocity() const
        {
            return _endVelocity;
        }

        HF_HDINLINE void setEnd(const FloatN& end, const FloatN& endVelocity)
        {
            _end = end;
            _endVelocity = endVelocity;
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
        HF_HDINLINE float closestTimeToPoint(const FloatN& point)
        {
            const auto a = point - _start;
            const auto b = _end - _start;
            return clamp(dot(a, b) / dot(b, b), 0.0f, 1.0f);
        }

        HF_HDINLINE void computeDistanceAndVelocity(const FloatN& point, float& dist, FloatN& velocity)
        {
            const auto a = point - _start;
            const auto b = _end - _start;
            const auto time = clamp(dot(a, b) / dot(b, b), 0.0f, 1.0f);
            
            dist = length(a - b * time) - _radius;
            velocity = mix(_startVelocity, _endVelocity, time);
        }

        HF_HDINLINE AABB getBoundingBox() const
        {
            const FloatN minPoint = min(_start, _end) - _radius;
            const FloatN maxPoint = max(_start, _end) + _radius;
            return AABB(minPoint, maxPoint);
        }

    private:
        FloatN _start;
        FloatN _end;
        FloatN _startVelocity;
        FloatN _endVelocity;
        float  _radius;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using CapsuleCollider2 = CapsuleCollider<2>;
    using CapsuleCollider3 = CapsuleCollider<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATION_FLUID_CAPSULE_OBSTACLE_HPP */
