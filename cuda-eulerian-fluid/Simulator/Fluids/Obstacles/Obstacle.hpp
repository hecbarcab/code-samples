#ifndef HF_SIMULATION_FLUID_OBSTACLE_HPP
#define HF_SIMULATION_FLUID_OBSTACLE_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Fluids/Obstacles/BoxCollider.hpp>
#include <Simulator/Fluids/Obstacles/CapsuleCollider.hpp>
#include <Simulator/Fluids/Obstacles/SphereCollider.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order, template <uint N> class _Collider>
    class Obstacle
    {
    public:
        using Collider = _Collider<_Order>;

    public:
        Obstacle()
            : _enabled(true)
        {
        }

    public:
        void setEnabled(bool enabled)
        {
            _enabled = enabled;
        }

        bool isEnabled() const
        {
            return _enabled;
        }

        void setCollider(const Collider& collider)
        {
            _collider = collider;
        }

        const Collider& getCollider() const
        {
            return _collider;
        }

    private:
        bool     _enabled;
        Collider _collider;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint O, template <uint N> class T> using ObstacleRef = Ref<Obstacle<O, T>>;

    template <template <uint N> class T> using Obstacle2 = Obstacle<2, T>;
    template <template <uint N> class T> using Obstacle3 = Obstacle<3, T>;
    using BoxObstacle2 = Obstacle2<BoxCollider>;
    using BoxObstacle3 = Obstacle3<BoxCollider>;
    using CircleObstacle2 = Obstacle2<SphereCollider>;
    using SphereObstacle3 = Obstacle3<SphereCollider>;
    using CapsuleObstacle2 = Obstacle2<CapsuleCollider>;
    using CapsuleObstacle3 = Obstacle3<CapsuleCollider>;

    template <template <uint N> class T> using Obstacle2Ref = Ref<Obstacle2<T>>;
    template <template <uint N> class T> using Obstacle3Ref = Ref<Obstacle3<T>>;
    using BoxObstacle2Ref = Ref<Obstacle2<BoxCollider>>;
    using BoxObstacle3Ref = Ref<Obstacle3<BoxCollider>>;
    using CircleObstacle2Ref = Ref<CircleObstacle2>;
    using SphereObstacle3Ref = Ref<SphereObstacle3>;
    using CapsuleObstacle2Ref = Ref<CapsuleObstacle2>;
    using CapsuleObstacle3Ref = Ref<CapsuleObstacle3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATION_FLUID_OBSTACLE_HPP */
