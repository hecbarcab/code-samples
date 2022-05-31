#ifndef HF_SIMULATOR_GEOMETRY_CAPSULE_HPP
#define HF_SIMULATOR_GEOMETRY_CAPSULE_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Geometry)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    class Capsule
    {
    public:
        static constexpr uint Order = _Order;
        using FloatN = FloatN<_Order>;

    public:
        Capsule()
            : _start(0.0f)
            , _end(0.0f)
            , _radius(10.0f)
        {
        }

        Capsule(const FloatN& start, const FloatN& end, float radius)
            : _start(start)
            , _end(end)
            , _radius(radius)
        {
        }

    public:
        HF_HDINLINE const FloatN& getStart() const
        {
            return _start;
        }

        HF_HDINLINE void setStart(const FloatN& start)
        {
            _start = start;
        }

        HF_HDINLINE const FloatN& getEnd() const
        {
            return _end;
        }

        HF_HDINLINE void setEnd(const FloatN& start)
        {
            _end = end;
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

        HF_HDINLINE float computeSignedDistance(const FloatN& point)
        {
            const auto time = closestTimeToPoint(point);
            const auto pointInCapsule = _start + (_end - _start) * time;
            return distance(point, pointInCapsule) - _radius;
        }

    private:
        FloatN _start;
        FloatN _end;
        float  _radius;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using Capsule2 = Capsule<2>;
    using Capsule3 = Capsule<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Geometry)

#endif /* HF_SIMULATOR_GEOMETRY_CAPSULE_HPP */
