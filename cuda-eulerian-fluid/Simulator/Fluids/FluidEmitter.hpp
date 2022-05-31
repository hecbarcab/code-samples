#ifndef HF_SIMULATION_FLUID_EMITTER_HPP
#define HF_SIMULATION_FLUID_EMITTER_HPP

#include <Simulator/Compute/Kernel.hpp>
#include <Simulator/Geometry/Primitives/AABB.hpp>
#include <Simulator/Fluids/Fluid.hpp>
#include <Simulator/Fluids/Kernels/EmissionKernel.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    /**
     * \brief Simple jet-like fluid emitter class.
     * \tparam _Order Order (dimensions) of the fluid to emit.
     */
    template <uint _Order>
    class FluidEmitter
    {
        static_assert(_Order >= 2 && _Order <= 3, "Only 2D and 3D fluid emitters are supported.");

    public:
        static constexpr uint Order = _Order;

        using Fluid    = Fluid<Order>;
        using FluidRef = typename Fluid::Ref;
        using Coords   = FloatN<_Order>;

    public:
        FluidEmitter() = default;

        FluidEmitter(const Coords& center, float radius, float falloff, const Coords& velocity)
            : _center(center)
            , _radius(radius)
            , _falloff(falloff)
            , _velocity(velocity)
            , _angularVelocity(0.0f)
        {
            
        }

    public:
        /**
         * \brief Gets the center of the emission (in world space coordinates).
         * \return Center of the emission.
         */
        const Coords& getCenter() const
        {
            return _center;
        }

        /**
         * \brief Sets the center of the emission (in world space coordinates).
         * \param center Center of the emission.
         */
        void setCenter(const Coords& center)
        {
            _center = center;
        }

        /**
         * \brief Gets the emission radius.
         * \return Emission radius.
         */
        float getRadius() const
        {
            return _radius;
        }

        /**
         * \brief Sets the emission radius.
         * \param radius Emission radius.
         */
        void setRadius(float radius)
        {
            _radius = radius;
        }

        /**
         * \brief Gets the emission falloff. This parameter controls the "softness" of the
         *        emitted fluid.
         * \return Emission falloff.
         */
        float getFalloff() const
        {
            return _falloff;
        }

        /**
         * \brief Sets the emission falloff. This parameter controls the "softness" of the emitted
         *        emitted fluid.
         * \param falloff Emission falloff.
         */
        void setFalloff(float falloff)
        {
            _falloff = falloff;
        }


        const Float4& getColor() const
        {
            return _color;
        }

        void setColor(const Float4& color)
        {
            _color = color;
        }

        /**
         * \brief Gets the emission velocity.
         * \return Emission velocity.
         */
        const Coords& getVelocity() const
        {
            return _velocity;
        }

        const Coords& getAngularVelocity() const
        {
            return _angularVelocity;
        }

        /**
         * \brief Set the emission velocity.
         * \param velocity 
         */
        void setVelocity(const Coords& velocity)
        {
            _velocity = velocity;
        }

        void setAngularVelocity(const Coords& angularVelocity)
        {
            _angularVelocity = angularVelocity;
        }

    public:
        template <typename LocationTag>
        void emit(const FluidRef& fluid, const LocationTag& location) const
        {
            const auto threadCount = fluid->getDomain().getDimsOfNodesGrid();

            Compute::Kernel::execute<Order, EmissionKernel>(location,
                                                            threadCount,
                                                            fluid->getDomain(),
                                                            fluid->getInkField()->getAccessor(location),
                                                            fluid->getVelocityField()->getAccessor(location),
                                                            _center,
                                                            _radius,
                                                            _falloff,
                                                            _velocity,
                                                            _angularVelocity,
                                                            _color);
        }

    private:
        Coords _center;
        float  _radius;
        float  _falloff;
        Coords _velocity;
        Coords _angularVelocity;
        Float4 _color;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using FluidEmitter2 = FluidEmitter<2>;
    using FluidEmitter3 = FluidEmitter<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATION_FLUID_EMITTER_HPP */
