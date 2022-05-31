#ifndef HF_SIMULATOR_GEOMETRY_RAY_HPP
#define HF_SIMULATOR_GEOMETRY_RAY_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Geometry/Primitives/Plane.hpp>

HF_BEGIN_NAMESPACE(HF, Geometry)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    class Ray
    {
    public:
        static constexpr uint Order = _Order;
        using FloatN = FloatN<_Order>;

    public:
        Ray()
            : _origin(0.0f)
            , _direction(0.0f)
        {
        }

        Ray(const FloatN& origin, const FloatN& direction)
            : _origin(origin)
            , _direction(direction)
        {
        }

    public:
        HF_HDINLINE const FloatN& getOrigin() const
        {
            return _origin;
        }

        HF_HDINLINE void setOrigin(const FloatN& origin)
        {
            _origin = origin;
        }

        HF_HDINLINE const FloatN& getDirection() const
        {
            return _direction;
        }

        HF_HDINLINE void setDirection(const FloatN& direction)
        {
            _direction = direction;
        }

    public:
        HF_HDINLINE FloatN getPoint(float distance)
        {
            return _origin + distance * _direction;
        }

        HF_HDINLINE float intersect(const Plane<Order>& plane)
        {
            const FloatN& normal = plane.getNormal();
            const float distance = plane.getDistance();
            return (distance - dot(normal, _origin)) / dot(normal, _direction);
        }

    private:
        FloatN _origin;
        FloatN _direction;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using Ray1 = Ray<1>;
    using Ray2 = Ray<2>;
    using Ray3 = Ray<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Geometry)

#endif /* HF_SIMULATOR_GEOMETRY_RAY_HPP */
