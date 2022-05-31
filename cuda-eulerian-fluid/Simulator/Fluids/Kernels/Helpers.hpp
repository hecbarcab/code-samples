#ifndef HF_SIMULATOR_FLUIDS_HELPERS_HPP
#define HF_SIMULATOR_FLUIDS_HELPERS_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Fluids/FluidBounds.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>
#include <Simulator/Fluids/FluidDomainBounds.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint Order>
    struct Helpers
    {
        using Domain               = FluidDomain<Order>;
        using DomainBounds         = FluidDomainBounds<Order>;
        using DomainBoundsVelocity = FluidDomainBoundsVelocity<Order>;
        using Coords               = FloatN<Order>;
        using Index                = UintN<Order>;

        template <typename VectorSampler>
        static HF_HDINLINE Coords getVectorAtPosition(const Domain& dom,
                                                      const VectorSampler& vectorField,
                                                      const Coords& pos)
        {
            Coords idx = dom.getCoords(pos);
            Coords value;

            for (uint i = 0; i < Order; ++i)
                value[i] = vectorField[i].getValue(idx);

            return value;
        }

        template <typename VectorAccessor>
        static HF_HDINLINE Coords getVectorAtCell(const VectorAccessor& vectorField,
                                                  const Index& idx)
        {
            Coords value;

            for (uint i = 0; i < Order; ++i)
                value[i] = vectorField[i].getValue(idx);

            return value;
        }

        template <typename VectorSampler>
        static HF_HDINLINE Coords getStaggeredVectorAtPosition(const Domain& dom,
                                                               const VectorSampler& vectorField,
                                                               const Coords& pos)
        {
            Coords value;

            for (uint i = 0; i < Order; ++i)
                value[i] = vectorField[i].getValue(dom.getFaceCoords(pos, i));

            return value;
        }

        template <typename VectorAccessor>
        static HF_HDINLINE Coords getStaggeredVectorAtCell(const VectorAccessor& vectorField,
                                                           const Index& idx)
        {
            Coords value;

            for (uint i = 0; i < Order; ++i)
            {
                Index nextIdx = idx; ++nextIdx[i];
                value[i] = 0.5f * (vectorField[i].getValue(nextIdx) + vectorField[i].getValue(idx));
            }

            return value;
        }

        template <typename VectorAccessor>
        static HF_HDINLINE float getStaggeredVectorAtCell(const VectorAccessor& vectorField,
                                                          const Index& idx,
                                                          int axis)
        {
            Index nextIdx = idx; ++nextIdx[axis];
            return 0.5f * (vectorField[axis].getValue(nextIdx) + vectorField[axis].getValue(idx));
        }

        template <typename VectorAccessor>
        static HF_HDINLINE Coords getStaggeredVectorAtFace(const VectorAccessor& vectorField,
                                                           const Index& idx,
                                                           int axis)
        {
            Coords value;

            for (uint i = 0; i < axis; ++i)
            {
                Index nextIdx = idx; ++nextIdx[i];
                value[i] = 0.5f * (vectorField[i].getValue(nextIdx) + vectorField[i].getValue(idx));
            }

            value[axis] = vectorField[axis].getValue(idx);

            for (uint i = axis + 1; i < Order; ++i)
            {
                Index nextIdx = idx; ++nextIdx[i];
                value[i] = 0.5f * (vectorField[i].getValue(nextIdx) + vectorField[i].getValue(idx));
            }

            return value;
        }

        template <typename BoundaryConstAccessor>
        static HF_HDINLINE FluidBounds getBoundaryAtCell(const Domain& dom,
                                                         const DomainBounds& domBounds,
                                                         const BoundaryConstAccessor& boundaryField,
                                                         const Index& idx)
        {
            // If the index lies outside of the domain, get the corresponding face and
            // retrieve the kind of boundary.

            const FluidDomainFace face = dom.getDomainFace(idx);
            
            if (face != FluidDomainFace::None) 
                return domBounds.getFaceBoundary(face);

            // Otherwise, check up the boundary field in case there's other moving
            // solid obstacle at the location.

            return boundaryField.getValue(idx) == 1
                 ? FluidBounds::Solid 
                 : FluidBounds::None;
        }

        template <typename VelocityConstAccessor>
        static HF_HDINLINE Coords getBoundaryVelocityAtCell(const Domain& dom,
                                                            const DomainBoundsVelocity& domVelocity,
                                                            const VelocityConstAccessor& boundaryVelocityField,
                                                            const Index& idx)
        {
            // If the index lies outside of the domain, get the corresponding face and
            // retrieve the boundary velocity.

            const FluidDomainFace face = dom.getDomainFace(idx);

            if (face != FluidDomainFace::None)
                return dom.getDomainFaceNormal(face) * domVelocity.getFaceVelocity(face);

            // Otherwise, directly grab the velocity stored at the boundary
            // velocity field.

            return getVectorAtCell(boundaryVelocityField, idx);
        }

        template <typename DistanceConstAccessor>
        static HF_HDINLINE Coords getBoundaryNormalAtCell(const Domain& dom,
                                                          const DistanceConstAccessor& boundaryDistanceField,
                                                          const Index& idx)
        {
            // If the index lies outside of the domain, get the corresponding face and
            // retrieve the boundary normal.

            const FluidDomainFace face = dom.getDomainFace(idx);

            if (face != FluidDomainFace::None)
                return dom.getDomainFaceNormal(face);
             
            // Otherwise, estimate the normal by using finite differences.

            Coords normal(0.0f);

            for (uint i = 0; i < Order; ++i)
            {
                Index prevIdx = idx; --prevIdx[i];
                Index nextIdx = idx; ++nextIdx[i];

                const float prevDist = boundaryDistanceField.getValue(prevIdx);
                const float nextDist = boundaryDistanceField.getValue(nextIdx);
                normal[i] = nextDist - prevDist;
            }

            const float len = length(normal);
            return normal / (len + 1e-5f);
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUIDS_HELPERS_HPP */
