#ifndef HF_SIMULATOR_GEOMETRY_SPHERE_HPP
#define HF_SIMULATOR_GEOMETRY_SPHERE_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Geometry/Primitives/Plane.hpp>

HF_BEGIN_NAMESPACE(HF, Geometry)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    class Sphere
    {
    public:
        static constexpr uint Order = _Order;
        using FloatN = FloatN<_Order>;

    public:
        Sphere()
            : _center(0.0f)
            , _radius(10.0f)
        {
        }

        Sphere(const FloatN& center, float radius)
            : _center(center)
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

        HF_HDINLINE float getRadius() const
        {
            return _radius;
        }

        HF_HDINLINE void setRadius(float radius)
        {
            _radius = radius;
        }

    public:
        HF_HDINLINE float computeSignedDistance(const FloatN& point)
        {
            return distance(point, _center) - _radius;
        }

    private:
        FloatN _center;
        float  _radius;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using Circle2 = Sphere<2>;
    using Sphere3 = Sphere<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Geometry)

#endif /* HF_SIMULATOR_GEOMETRY_SPHERE_HPP */
