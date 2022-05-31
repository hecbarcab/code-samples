#ifndef HF_SIMULATION_FLUID_HPP
#define HF_SIMULATION_FLUID_HPP

#include <Simulator/Compute/Kernel.hpp>
#include <Simulator/Compute/Copy.hpp>
#include <Simulator/Compute/Buffers/Buffer.hpp>
#include <Simulator/Geometry/Primitives/AABB.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>
#include <Simulator/Fluids/FluidDomainBounds.hpp>
#include <Simulator/Fluids/FluidDomainBoundsVelocity.hpp>
#include <Simulator/Fluids/FluidScalarField.hpp>
#include <Simulator/Fluids/FluidVectorField.hpp>
#include <Simulator/Fluids/FluidProperty.hpp>
#include <Simulator/Fluids/FluidParams.hpp>
#include <Simulator/Fluids/Obstacles/SphereCollider.hpp>
#include <Simulator/Fluids/Obstacles/CapsuleCollider.hpp>
#include <Simulator/Fluids/Obstacles/Obstacle.hpp>
#include <Simulator/Fluids/Kernels/AdvectionKernel.hpp>
#include <Simulator/Fluids/Kernels/AdvectionMacCormackKernel.hpp>
#include <Simulator/Fluids/Kernels/DissipationKernel.hpp>
#include <Simulator/Fluids/Kernels/ForceKernel.hpp>
#include <Simulator/Fluids/Kernels/GravityKernel.hpp>
#include <Simulator/Fluids/Kernels/PressureJacobiKernel.hpp>
#include <Simulator/Fluids/Kernels/PressureJacobiProjectionKernel.hpp>
#include <Simulator/Fluids/Kernels/ObstacleBoundaryKernel.hpp>
#include <Simulator/Fluids/Kernels/VelocityAdvectionKernel.hpp>
#include <Simulator/Fluids/Kernels/VelocityBoundaryProjection.hpp>
#include <Simulator/Fluids/Kernels/VelocityDivergenceKernel.hpp>
#include <Simulator/Fluids/Kernels/ViscosityKernel.hpp>
#include <Simulator/Fluids/Kernels/VorticityConfinementKernel.hpp>
#include <Simulator/Fluids/Kernels/VorticityKernel.hpp>
#include <Simulator/Fluids/Kernels/SamplingKernel.hpp>
#include <Simulator/Utility/Patterns/DoubleBuffer.hpp>
#include <Simulator/Utility/Patterns/TripleBuffer.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    class Fluid
    {
        static_assert(_Order >= 2 && _Order <= 3, "Only 2D and 3D fluids are supported.");

    public:
        static constexpr uint Order = _Order;

        using Ref                        = Ref<Fluid>;
        using Dims                       = IntN<_Order>;
        using Index                      = IntN<_Order>;
        using Coords                     = FloatN<_Order>;
        using AABB                       = Geometry::AABB<_Order>;
        using Domain                     = FluidDomain<_Order>;
        using DomainBounds               = FluidDomainBounds<_Order>;
        using DomainBoundsVelocity       = FluidDomainBoundsVelocity<_Order>;
        using Params                     = FluidParams<_Order>;
                                         
        using InkField                   = FluidScalarField<_Order, Float4>;
        using TemperatureField           = FluidScalarField<_Order, float>;
        using PressureField              = FluidScalarField<_Order, float>;
        using PressureGradNormField      = FluidScalarField<_Order, float>;
        using VelocityField              = FluidVectorField<_Order, float>;
        using VelocityDivergenceField    = FluidScalarField<_Order, float>;
        using BoundaryField              = FluidScalarField<_Order, uchar>;
        using BoundaryDistanceField      = FluidScalarField<_Order, float>;
        using BoundaryVelocityField      = FluidVectorField<_Order, float>;
        using VorticityField             = FluidVectorField<_Order, float>;
        using VorticityNormField         = FluidScalarField<_Order, float>;
        using ConfinementField           = FluidVectorField<_Order, float>;

        using InkFieldRef                = typename InkField::Ref;
        using TemperatureFieldRef        = typename TemperatureField::Ref;
        using PressureFieldRef           = typename PressureField::Ref;
        using PressureGradNormFieldRef   = typename PressureGradNormField::Ref;
        using VelocityFieldRef           = typename VelocityField::Ref;
        using VelocityDivergenceFieldRef = typename VelocityDivergenceField::Ref;
        using BoundaryFieldRef           = typename BoundaryField::Ref;
        using BoundaryDistanceFieldRef   = typename BoundaryDistanceField::Ref;
        using BoundaryVelocityFieldRef   = typename BoundaryVelocityField::Ref;
        using VorticityFieldRef          = typename VorticityField::Ref;
        using VorticityNormFieldRef      = typename VorticityNormField::Ref;
        using ConfinementFieldRef        = typename ConfinementField::Ref;

        using BoxObstacle                = Obstacle<_Order, BoxCollider>;
        using SphereObstacle             = Obstacle<_Order, SphereCollider>;
        using CapsuleObstacle            = Obstacle<_Order, CapsuleCollider>;
        using BoxObstacleRef             = ObstacleRef<_Order, BoxCollider>;
        using SphereObstacleRef          = ObstacleRef<_Order, SphereCollider>;
        using CapsuleObstacleRef         = ObstacleRef<_Order, CapsuleCollider>;

    protected:
        Fluid(const Params& params)
            : _domain(params.region, params.dims)
            , _timestep(params.timestep)
            , _gravity(params.gravity)
            , _density(params.density)
            , _viscosity(params.viscosity)
            , _confinement(params.confinement)
            , _jacobiSteps(params.jacobiSteps)
            , _inkDissipation(params.inkDissipation)
            , _velocityDissipation(params.velocityDissipation)
            , _pressureDissipation(params.pressureDissipation)
        {
            // Allocate grids.

            _inkField[0]             = InkField::Create(_domain);
            _inkField[1]             = InkField::Create(_domain);
            _inkField[2]             = InkField::Create(_domain);
            _temperatureField[0]     = TemperatureField::Create(_domain);
            _temperatureField[1]     = TemperatureField::Create(_domain);
            _pressureField[0]        = PressureField::Create(_domain);
            _pressureField[1]        = PressureField::Create(_domain);
            _pressureGradNormField   = PressureGradNormField::Create(_domain);
            _velocityField[0]        = VelocityField::Create(_domain, true);
            _velocityField[1]        = VelocityField::Create(_domain, true);
            _velocityDivergenceField = VelocityDivergenceField::Create(_domain);
            _boundaryField           = BoundaryField::Create(_domain);
            _boundaryDistanceField   = BoundaryDistanceField::Create(_domain);
            _boundaryVelocityField   = BoundaryVelocityField::Create(_domain, false);
            _vorticityField          = VorticityField::Create(_domain, false);
            _vorticityNormField      = VorticityNormField::Create(_domain);
            _confinementField        = ConfinementField::Create(_domain, false);

            // Initialize them all to zero.

            _inkField[0]->clear(Float4(0.0f));
            _inkField[1]->clear(Float4(0.0f));
            _inkField[2]->clear(Float4(0.0f));
            _temperatureField[0]->clear(0.0f);
            _temperatureField[1]->clear(0.0f);
            _pressureField[0]->clear(0.0f);
            _pressureField[1]->clear(0.0f);
            _pressureGradNormField->clear(0.0f);
            _velocityField[0]->getAxis(0)->clear(0.0f);
            _velocityField[1]->getAxis(0)->clear(0.0f);
            _velocityField[0]->getAxis(1)->clear(0.0f);
            _velocityField[1]->getAxis(1)->clear(0.0f);
            _velocityField[0]->getAxis(2)->clear(0.0f);
            _velocityField[1]->getAxis(2)->clear(0.0f);
            _velocityDivergenceField->clear(0.0f);
            _boundaryField->clear(0);
            _boundaryDistanceField->clear(0.0f);
            _boundaryVelocityField->getAxis(0)->clear(0.0f);
            _boundaryVelocityField->getAxis(1)->clear(0.0f);
            _boundaryVelocityField->getAxis(2)->clear(0.0f);
            _vorticityField->getAxis(0)->clear(0.0f);
            _vorticityField->getAxis(1)->clear(0.0f);
            _vorticityField->getAxis(2)->clear(0.0f);
            _vorticityNormField->clear(0.0f);
            _confinementField->getAxis(0)->clear(0.0f);
            _confinementField->getAxis(1)->clear(0.0f);
            _confinementField->getAxis(2)->clear(0.0f);
        }

    public:
        ~Fluid()
        {
        }

    public:
        const Domain& getDomain() const
        {
            return _domain;
        }

        DomainBounds& getDomainBounds()
        {
            return _domainBounds;
        }

        const DomainBounds& getDomainBounds() const
        {
            return _domainBounds;
        }

        DomainBoundsVelocity& getDomainBoundsVelocity()
        {
            return _domainBoundsVelocity;
        }

        const DomainBoundsVelocity& getDomainBoundsVelocity() const
        {
            return _domainBoundsVelocity;
        }

        float getTimestep() const
        {
            return _timestep;
        }

        Coords getGravity() const
        {
            return _gravity;
        }

        float getRestDensity() const
        {
            return _density;
        }

        float getViscosity() const
        {
            return _viscosity;
        }

        const InkFieldRef& getInkField() const
        {
            return _inkField.getFront();
        }

        const TemperatureFieldRef& getTemperatureField() const
        {
            return _temperatureField.getFront();
        }
        
        const PressureFieldRef& getPressureField() const
        {
            return _pressureField.getFront();
        }

        const PressureGradNormFieldRef& getPressureGradNormField() const
        {
            return _pressureGradNormField;
        }
        
        const VelocityFieldRef& getVelocityField() const
        {
            return _velocityField.getFront();
        }
        
        const VelocityDivergenceFieldRef& getVelocityDivergenceField() const
        {
            return _velocityDivergenceField;
        }
        
        const BoundaryFieldRef& getBoundaryField() const
        {
            return _boundaryField;
        }

        const BoundaryDistanceFieldRef& getBoundaryDistanceField() const
        {
            return _boundaryDistanceField;
        }
        
        const BoundaryVelocityFieldRef& getBoundaryVelocityField() const
        {
            return _boundaryVelocityField;
        }

        const VorticityFieldRef& getVorticityField() const
        {
            return _vorticityField;
        }

        const VorticityNormFieldRef& getVorticityNormField() const
        {
            return _vorticityNormField;
        }

        const ConfinementFieldRef& getConfinementField() const
        {
            return _confinementField;
        }

        BoxObstacleRef createBoxObstacle()
        {
            auto box = std::make_shared<BoxObstacle>();
            _boxObstacles.push_back(box);
            return box;
        }

        SphereObstacleRef createSphereObstacle()
        {
            auto sphere = std::make_shared<SphereObstacle>();
            _sphereObstacles.push_back(sphere);
            return sphere;
        }

        CapsuleObstacleRef createCapsuleObstacle()
        {
            auto capsule = std::make_shared<CapsuleObstacle>();
            _capsuleObstacles.push_back(capsule);
            return capsule;
        }

        void removeBoxObstacle(const BoxObstacleRef& box)
        {
            const auto it = std::find(_boxObstacles.begin(), _boxObstacles.end(), box);

            if (it != _boxObstacles.end())
                _boxObstacles.remove(it);
        }

        void removeSphereObstacle(const SphereObstacleRef& sphere)
        {
            const auto it = std::find(_sphereObstacles.begin(), _sphereObstacles.end(), sphere);

            if (it != _sphereObstacles.end())
                _sphereObstacles.remove(it);
        }

        void removeCapsuleObstacle(const CapsuleObstacleRef& sphere)
        {
            const auto it = std::find(_capsuleObstacles.begin(), _capsuleObstacles.end(), sphere);

            if (it != _capsuleObstacles.end())
                _capsuleObstacles.remove(it);
        }

        AABB getObstaclesBoundingBox() const
        {
            AABB boundingBox;

            for (const auto& sphere : _sphereObstacles)
                boundingBox.extend(sphere->getCollider().getBoundingBox());

            for (const auto& capsule : _capsuleObstacles)
                boundingBox.extend(capsule->getCollider().getBoundingBox());

            return boundingBox;
        }

    private:
        template <typename LocationTag, typename FieldRef>
        void advectScalarField(const LocationTag& location, FieldRef& field, float timestep)
        {
            auto& velocityField = _velocityField.getFront();
            auto& frontField = field.getFront();   // N
            auto& middleField = field.getMiddle(); // N+1 (hat) / N+1
            auto& backField = field.getBack();     // N (hat)

            // N+1 (hat)

            Compute::Kernel::execute<Order, AdvectionKernel>(location,
                                                             _domain.getDims(),
                                                             _domain,
                                                             frontField->getSampler(location),
                                                             velocityField->getConstAccessor(location),
                                                             middleField->getAccessor(location),
                                                             timestep);
            /*
            // N (hat)

            Compute::Kernel::execute<Order, AdvectionKernel>(location,
                                                             _domain.getDims(),
                                                             _domain,
                                                             middleField->getSampler(location),
                                                             velocityField->getConstAccessor(location),
                                                             backField->getAccessor(location),
                                                             -timestep);
            
            // N+1

            Compute::Kernel::execute<Order, AdvectionMacCormackKernel>(location,
                                                                       _domain.getDims(),
                                                                       _domain,
                                                                       frontField->getSampler(location),
                                                                       backField->getAccessor(location),
                                                                       velocityField->getConstAccessor(location),
                                                                       middleField->getAccessor(location),
                                                                       timestep);
            */

            field.swap();
        }

        template <typename LocationTag>
        void advectVelocityField(const LocationTag& location, float timestep, float dissipation)
        {
            Compute::Kernel::execute<Order, VelocityAdvectionKernel>(location,
                                                                     _domain.getDimsOfNodesGrid(),
                                                                     _domain,
                                                                     _velocityField.getFront()->getSampler(location),
                                                                     _velocityField.getBack()->getAccessor(location),
                                                                     timestep,
                                                                     dissipation);

            _velocityField.swap();
        }

        template <typename LocationTag, typename FieldRef>
        void applyForces(const LocationTag& location, FieldRef& field, float timestep)
        {
            Compute::Kernel::execute<Order, ForceKernel>(location,
                                                         _domain.getDimsOfNodesGrid(),
                                                         _domain,
                                                         field->getConstAccessor(location),
                                                         _velocityField.getFront()->getAccessor(location),
                                                         timestep);
        }

        template <typename LocationTag, typename FieldRef, typename Dissipation>
        void applyDissipation(const LocationTag& location, FieldRef& field, float timestep, Dissipation dissipation)
        {
            Compute::Kernel::execute<Order, DissipationKernel>(location,
                                                               _domain.getDims(),
                                                               _domain,
                                                               field->getAccessor(location),
                                                               timestep * dissipation);
        }

        template <typename LocationTag>
        void applyGravityForces(const LocationTag& location, float timestep)
        {
            Compute::Kernel::execute<Order, GravityKernel>(location,
                                                           _domain.getDimsOfNodesGrid(),
                                                           _domain,
                                                           _velocityField.getFront()->getAccessor(location),
                                                           _gravity,
                                                           timestep);
        }

        template <typename LocationTag>
        void applyViscosityForces(const LocationTag& location, float timestep, float viscosity)
        {
            Compute::Kernel::execute<Order, ViscosityKernel>(location,
                                                             _domain.getDimsOfNodesGrid() + 5,
                                                             _domain,
                                                             _velocityField.getFront()->getConstAccessor(location),
                                                             _velocityField.getBack()->getAccessor(location),
                                                             viscosity * timestep / _density);

            _velocityField.swap();
        }

        template <typename LocationTag>
        void computeDivergence(const LocationTag& location)
        {
            Compute::Kernel::execute<Order, VelocityDivergenceKernel>(location,
                                                                      _domain.getDims(),
                                                                      _domain,
                                                                      _velocityField.getFront()->getConstAccessor(location),
                                                                      _velocityDivergenceField->getAccessor(location));
        }

        template <typename LocationTag>
        void computeVorticity(const LocationTag& location)
        {
            Compute::Kernel::execute<Order, VorticityKernel>(location,
                                                             _domain.getDims(),
                                                             _domain,
                                                             _velocityField.getFront()->getConstAccessor(location),
                                                             _vorticityField->getAccessor(location),
                                                             _vorticityNormField->getAccessor(location));
        }

        template <typename LocationTag>
        void computeConfinement(const LocationTag& location)
        {
            Compute::Kernel::execute<Order, VorticityConfinementKernel>(location,
                                                                        _domain.getDims(),
                                                                        _domain,
                                                                        _vorticityField->getConstAccessor(location),
                                                                        _vorticityNormField->getConstAccessor(location),
                                                                        _confinementField->getAccessor(location),
                                                                        _confinement);
        }

        template <typename LocationTag>
        void computePressure(const LocationTag& location, float timestep)
        {
            //_pressureField.getFront()->clear(location, 0.0f);

            applyDissipation(location, _pressureField.getFront(), _timestep, _pressureDissipation);

            for (uint i = 0; i < _jacobiSteps; ++i)
            {
                Compute::Kernel::execute<Order, PressureJacobiKernel>(location,
                                                                      _domain.getDims(),
                                                                      _domain,
                                                                      _domainBounds,
                                                                      _pressureField.getFront()->getConstAccessor(location),
                                                                      _velocityDivergenceField->getConstAccessor(location),
                                                                      _boundaryField->getConstAccessor(location),
                                                                      _pressureField.getBack()->getAccessor(location),
                                                                      _density / timestep);
                _pressureField.swap();
            }
        }

        template <typename LocationTag>
        void applyPressureForces(const LocationTag& location, float timestep)
        {
            Compute::Kernel::execute<Order, PressureJacobiProjectionKernel>(location,
                                                                            _domain.getDimsOfNodesGrid(),
                                                                            _domain,
                                                                            _domainBounds,
                                                                            _pressureField.getFront()->getConstAccessor(location),
                                                                            _boundaryField->getConstAccessor(location),
                                                                            _velocityField.getFront()->getAccessor(location),
                                                                            _pressureGradNormField->getAccessor(location),
                                                                            timestep / _density);
        }

        template <typename LocationTag>
        void enforceBoundaryConditions(const LocationTag& location)
        {
            Compute::Kernel::execute<Order, VelocityBoundaryProjectionKernel>(location,
                                                                              _domain.getDimsOfNodesGrid(),
                                                                              _domain,
                                                                              _domainBounds,
                                                                              _domainBoundsVelocity,
                                                                              _boundaryField->getConstAccessor(location),
                                                                              _boundaryDistanceField->getConstAccessor(location),
                                                                              _boundaryVelocityField->getConstAccessor(location),
                                                                              _velocityField.getFront()->getAccessor(location));
        }

        template <typename LocationTag>
        void rasterizeObstacles(const LocationTag& location)
        {
            // Clear boundary field -- Only boundary and distances are required to be cleared,
            // the velocity will be overwritten at the necessary locations

            _boundaryField->clear(location, false);
            _boundaryDistanceField->clear(location, 1e10);
            _boundaryVelocityField->getAxis(0)->clear(location, 0.0f);
            _boundaryVelocityField->getAxis(1)->clear(location, 0.0f);
            _boundaryVelocityField->getAxis(2)->clear(location, 0.0f);

            // Compute narrow-band threshold.

            const float narrowBandThreshold = 2.0f * length(_domain.getDx());

            // Rasterize obstacles. First the spheres.
            {
                for (uint i = 0; i < _sphereObstacles.size(); ++i)
                {
                    if (!_sphereObstacles[i]->isEnabled())
                        continue;

                    const auto& sphere = _sphereObstacles[i]->getCollider();

                    // Determine obstacle's AABB and determine which subregion of the domain
                    // it will affect.

                    const Coords minPoint = sphere.getCenter() - (sphere.getRadius() + narrowBandThreshold);
                    const Coords maxPoint = sphere.getCenter() + (sphere.getRadius() + narrowBandThreshold);

                    const Coords minCoords = floor(_domain.getCellCoords(minPoint));
                    const Coords maxCoords = ceil(_domain.getCellCoords(maxPoint));

                    const Index minIndex = _domain.getCellIndex(minCoords, true);
                    const Index maxIndex = _domain.getCellIndex(maxCoords, true);

                    const Index subRegionOffset = _domain.getCellIndex(minCoords, true);
                    const Index subRegionSize   = _domain.getCellIndex(maxCoords, true) - subRegionOffset + Index(1);

                    // Rasterize obstacle!

                    Compute::Kernel::execute<Order, ObstacleBoundaryKernel>(location,
                                                                            _domain.getDims(),
                                                                            _domain,
                                                                            subRegionOffset,
                                                                            subRegionSize,
                                                                            _boundaryField->getAccessor(location),
                                                                            _boundaryDistanceField->getAccessor(location),
                                                                            _boundaryVelocityField->getAccessor(location),
                                                                            sphere,
                                                                            narrowBandThreshold);
                }
            }
            // Then the capsules.
            {
                for (uint i = 0; i < _capsuleObstacles.size(); ++i)
                {
                    if (!_capsuleObstacles[i]->isEnabled())
                        continue;

                    const auto& capsule = _capsuleObstacles[i]->getCollider();

                    // Determine obstacle's AABB and determine which subregion of the domain
                    // it will affect.

                    const Coords minPoint = HF::min(capsule.getStart(), capsule.getEnd()) - (capsule.getRadius() + narrowBandThreshold);
                    const Coords maxPoint = HF::max(capsule.getStart(), capsule.getEnd()) + (capsule.getRadius() + narrowBandThreshold);

                    const Coords minCoords = floor(_domain.getCellCoords(minPoint));
                    const Coords maxCoords = ceil(_domain.getCellCoords(maxPoint));

                    const Index minIndex = _domain.getCellIndex(minCoords, true);
                    const Index maxIndex = _domain.getCellIndex(maxCoords, true);

                    const Index subRegionOffset = _domain.getCellIndex(minCoords, true);
                    const Index subRegionSize   = _domain.getCellIndex(maxCoords, true) - subRegionOffset + Index(1);

                    // Rasterize obstacle!

                    Compute::Kernel::execute<Order, ObstacleBoundaryKernel>(location,
                                                                            _domain.getDims(),
                                                                            _domain,
                                                                            subRegionOffset,
                                                                            subRegionSize,
                                                                            _boundaryField->getAccessor(location),
                                                                            _boundaryDistanceField->getAccessor(location),
                                                                            _boundaryVelocityField->getAccessor(location),
                                                                            capsule,
                                                                            narrowBandThreshold);
                }
            }
            // End with boxes.
            {
                for (uint i = 0; i < _boxObstacles.size(); ++i)
                {
                    if (!_boxObstacles[i]->isEnabled())
                        continue;

                    const auto& box = _boxObstacles[i]->getCollider();

                    // Determine obstacle's AABB and determine which subregion of the domain
                    // it will affect.

                    const AABB aabb = box.getBoundingBox();

                    const Coords minPoint = aabb.getMinPoint() - narrowBandThreshold;
                    const Coords maxPoint = aabb.getMaxPoint() + narrowBandThreshold;

                    const Coords minCoords = floor(_domain.getCellCoords(minPoint));
                    const Coords maxCoords = ceil(_domain.getCellCoords(maxPoint));

                    const Index minIndex = _domain.getCellIndex(minCoords, true);
                    const Index maxIndex = _domain.getCellIndex(maxCoords, true);

                    const Index subRegionOffset = _domain.getCellIndex(minCoords, true);
                    const Index subRegionSize   = _domain.getCellIndex(maxCoords, true) - subRegionOffset + Index(1);

                    // Rasterize obstacle!

                    Compute::Kernel::execute<Order, ObstacleBoundaryKernel>(location,
                                                                            _domain.getDims(),
                                                                            _domain,
                                                                            subRegionOffset,
                                                                            subRegionSize,
                                                                            _boundaryField->getAccessor(location),
                                                                            _boundaryDistanceField->getAccessor(location),
                                                                            _boundaryVelocityField->getAccessor(location),
                                                                            box,
                                                                            narrowBandThreshold);
                }
            }
        }

    public:
        template <typename LocationTag>
        void clear(const LocationTag& location)
        {
            _inkField.getFront()->clear(location, Float4(0.0f));
            _temperatureField.getFront()->clear(location, 0.0f);
            _pressureField.getFront()->clear(location, 0.0f);
            _velocityField.getFront()->getAxis(0)->clear(location, 0.0f);
            _velocityField.getFront()->getAxis(1)->clear(location, 0.0f);
            _velocityField.getFront()->getAxis(2)->clear(location, 0.0f);
            _boundaryField->clear(location, 0);
            _boundaryDistanceField->clear(location, 0.0f);
            _boundaryVelocityField->getAxis(0)->clear(location, 0.0f);
            _boundaryVelocityField->getAxis(1)->clear(location, 0.0f);
            _boundaryVelocityField->getAxis(2)->clear(location, 0.0f);
        }

        template <typename LocationTag>
        void step(const LocationTag& location)
        {
            // 0) Rasterize obstacles

            rasterizeObstacles(location);

            // 1) Advect property fields

            advectScalarField(location, _inkField, _timestep);
            applyDissipation(location, _inkField.getFront(), _timestep, _inkDissipation);

            advectVelocityField(location, _timestep, _velocityDissipation);

            // 2) Internal & external body forces
            
            if (dot(_gravity, _gravity) > 0.0f)
                applyGravityForces(location, _timestep);

            if (_viscosity > 0.0f)
                applyViscosityForces(location, _timestep, _viscosity);

            // 3) Apply pressure projection

            //enforceBoundaryConditions(location);    // enfore before (proper divergence)

            computeDivergence(location);
            computePressure(location, _timestep);
            applyPressureForces(location, _timestep);

            // 4) Enforce boundary conditions

            enforceBoundaryConditions(location);

            if (_confinement > 0.0f)
            {
                computeVorticity(location);
                computeConfinement(location);
                applyForces(location, _confinementField, _timestep);
            }
        }

        template <typename LocationTag>
        void dump(const LocationTag& location, const std::string& baseFilename, bool ink, bool pressure, bool velocity, bool vorticity)
        {
            if (ink)
            {
                //const std::string filename = baseFilename + "_ink.npy";
                //_inkField.getFront()->saveToFile(location, filename);
                //std::cout << format("Writing ink field to \"%s\"...", filename) << std::endl;
            }

            if (pressure)
            {
                const std::string filename = baseFilename + "_pressure.npy";
                _pressureField.getFront()->saveToFile(location, filename);

                std::cout << format("Writing pressure field to \"%s\"...", filename) << std::endl;
            }

            if (velocity)
            {
                const std::string filename = baseFilename + "_velocity?.npy";
                const std::string filenameX = baseFilename + "_velocityX.npy";
                const std::string filenameY = baseFilename + "_velocityY.npy";
                const std::string filenameZ = baseFilename + "_velocityZ.npy";
                _velocityField.getFront()->saveToFile(location, filenameX, 0);
                _velocityField.getFront()->saveToFile(location, filenameY, 1);
                _velocityField.getFront()->saveToFile(location, filenameZ, 2);

                std::cout << format("Writing velocity field to \"%s\"...", filename) << std::endl;
            }

            if (vorticity)
            {
                const std::string filename = baseFilename + "_vorticity?.npy";
                const std::string filenameX = baseFilename + "_vorticityX.npy";
                const std::string filenameY = baseFilename + "_vorticityY.npy";
                const std::string filenameZ = baseFilename + "_vorticityZ.npy";
                _vorticityField->saveToFile(location, filenameX, 0);
                _vorticityField->saveToFile(location, filenameY, 1);
                _vorticityField->saveToFile(location, filenameZ, 2);

                std::cout << format("Writing vorticity field to \"%s\"...", filename) << std::endl;
            }
        }

    private:
        Domain                            _domain;
        DomainBounds                      _domainBounds;
        DomainBoundsVelocity              _domainBoundsVelocity;
        float                             _timestep;
        Coords                            _gravity;
        float                             _density;
        float                             _viscosity;
        float                             _confinement;
        uint                              _jacobiSteps;
        Float4                            _inkDissipation;
        float                             _velocityDissipation;
        float                             _pressureDissipation;

        TripleBuffer<InkFieldRef>         _inkField;
        DoubleBuffer<TemperatureFieldRef> _temperatureField;
        DoubleBuffer<PressureFieldRef>    _pressureField;
        PressureGradNormFieldRef          _pressureGradNormField;
        DoubleBuffer<VelocityFieldRef>    _velocityField;
        VelocityDivergenceFieldRef        _velocityDivergenceField;
        BoundaryFieldRef                  _boundaryField;
        BoundaryDistanceFieldRef          _boundaryDistanceField;
        BoundaryVelocityFieldRef          _boundaryVelocityField;
        VorticityFieldRef                 _vorticityField;
        VorticityNormFieldRef             _vorticityNormField;
        ConfinementFieldRef               _confinementField;
            
        std::vector<BoxObstacleRef>       _boxObstacles;
        std::vector<SphereObstacleRef>    _sphereObstacles;
        std::vector<CapsuleObstacleRef>   _capsuleObstacles;

    public:
        static Ref Create(const Params& params)
        {
            return Ref(new Fluid(params));
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using Fluid2 = Fluid<2>;
    using Fluid3 = Fluid<3>;

    using Fluid2Ref = Fluid2::Ref;
    using Fluid3Ref = Fluid3::Ref;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATION_FLUID_HPP */
