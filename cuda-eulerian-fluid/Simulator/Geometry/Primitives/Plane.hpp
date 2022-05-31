#ifndef HF_SIMULATOR_GEOMETRY_PLANE_HPP
#define HF_SIMULATOR_GEOMETRY_PLANE_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Geometry)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    class Plane
    {
    public:
        static constexpr uint Order = _Order;
        using FloatN = FloatN<_Order>;
        
    public:
        Plane()
            : _normal(0.0f)
            , _distance(0.0f)
        {
        }

        Plane(const FloatN& point, const FloatN& normal)
            : _normal(normal)
            , _distance(-dot(point, normal))
        {
        }

        Plane(const FloatN& normal, float distance)
            : _normal(normal)
            , _distance(distance)
        {
        }

    public:
        HF_HDINLINE const FloatN& getNormal() const
        {
            return _normal;
        }

        HF_HDINLINE void setNormal(const FloatN& normal)
        {
            _normal = normal;
        }

        HF_HDINLINE float getDistance() const
        {
            return _distance;
        }

        HF_HDINLINE void setDistance(float distance)
        {
            _distance = distance;
        }

        HF_HDINLINE HF::FloatN<Order + 1> toFloatVector() const
        {
            return HF::FloatN<Order + 1>(_normal, _distance);
        }

    public:
        HF_HDINLINE float distanceToPlane(const FloatN& point)
        {
            return dot(point, _normal) + distance;
        }

        HF_HDINLINE FloatN project(const FloatN& point)
        {
            return _normal * distanceToPlane(point);
        }

        HF_HDINLINE FloatN projectOntoPlane(const FloatN& point)
        {
            return point - project(point);
        }

    private:
        FloatN _normal;
        float  _distance;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using Plane1 = Plane<1>;
    using Plane2 = Plane<2>;
    using Plane3 = Plane<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Geometry)

#endif /* HF_SIMULATOR_GEOMETRY_PLANE_HPP */
