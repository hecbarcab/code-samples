#ifndef HF_SIMULATOR_FLUIDS_VORTICITY_HPP
#define HF_SIMULATOR_FLUIDS_VORTICITY_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/KernelConfig.hpp>
#include <Simulator/Compute/KernelThread.hpp>
#include <Simulator/Compute/KernelBlockDims.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint Order, typename LocationTag>
    struct VorticityKernel;

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename LocationTag>
    struct VorticityKernel<2, LocationTag>
    {
        using Config = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;

        using Thread = Compute::KernelThread2;
        using Domain = FluidDomain2;
        using Coords = Float2;
        using Index  = Int2;

        template <typename VelocityConstAccessor, typename VorticityAccessor,
                  typename VorticityNormAccessor>
        static HF_HDINLINE void kernel(Thread                thread,
                                       Domain                dom,
                                       VelocityConstAccessor velocityField,
                                       VorticityAccessor     vorticityField,
                                       VorticityNormAccessor vorticityNormField)
        {
            if (any(greaterThanEqual(thread.index, dom.getDims())))
                return;

            const Coords oneOverDx = dom.getOneOverDx();

            // Compute velocity curl at the center of the cell

            // w = ∇×u = ∂uy/∂x - ∂ux/∂

            const float duy_dx = oneOverDx[0] * (velocityField[1].getValue(thread.index + Int2(1, 0)) - velocityField[1].getValue(thread.index));
            const float dux_dy = oneOverDx[1] * (velocityField[0].getValue(thread.index + Int2(0, 1)) - velocityField[0].getValue(thread.index));
            
            const float w = duy_dx - dux_dy;

            vorticityField[0].setValue(thread.index, w);
            vorticityNormField.setValue(abs(w));
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename LocationTag>
    struct VorticityKernel<3, LocationTag>
    {
        using Config = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;

        using Thread  = Compute::KernelThread3;
        using Domain  = FluidDomain3;
        using Coords  = Float3;
        using Index   = Int3;
        using Helpers = Helpers<3>;

        template <typename VelocityConstAccessor,
                  typename VorticityAccessor,
                  typename VorticityNormAccessor>
        static HF_HDINLINE void kernel(Thread                thread,
                                       Domain                dom,
                                       VelocityConstAccessor velocityField,
                                       VorticityAccessor     vorticityField,
                                       VorticityNormAccessor vorticityNormField)
        {
            if (any(greaterThanEqual(thread.index, dom.getDims())))
                return;

            const Coords oneOverDx = dom.getOneOverDx();

            // Compute velocity curl at the center of the cell

            // w = ∇×u = [ ∂uz/∂y - ∂uy/∂z
            //              ∂ux/∂z - ∂uz/∂x,
            //              ∂uy/∂x - ∂ux/∂y ]

            /*
            const float duz_dy = oneOverDx[1] * (velocityField[2].getValue(thread.index + Int3(0, 1, 0)) - velocityField[2].getValue(thread.index));
            const float duy_dz = oneOverDx[2] * (velocityField[1].getValue(thread.index + Int3(0, 0, 1)) - velocityField[1].getValue(thread.index));
            const float dux_dz = oneOverDx[2] * (velocityField[0].getValue(thread.index + Int3(0, 0, 1)) - velocityField[0].getValue(thread.index));
            const float duz_dx = oneOverDx[0] * (velocityField[2].getValue(thread.index + Int3(1, 0, 0)) - velocityField[2].getValue(thread.index));
            const float duy_dx = oneOverDx[0] * (velocityField[1].getValue(thread.index + Int3(1, 0, 0)) - velocityField[1].getValue(thread.index));
            const float dux_dy = oneOverDx[1] * (velocityField[0].getValue(thread.index + Int3(0, 1, 0)) - velocityField[0].getValue(thread.index));
            const Coords w = Coords(duz_dy - duy_dz, dux_dz - duz_dx, duy_dx - dux_dy);
            */

            const float duz_dy = oneOverDx[1] * (Helpers::getStaggeredVectorAtCell(velocityField, thread.index + Int3(0,  1, 0), 2) -
                                                 Helpers::getStaggeredVectorAtCell(velocityField, thread.index + Int3(0, -1, 0), 2));
            const float duy_dz = oneOverDx[2] * (Helpers::getStaggeredVectorAtCell(velocityField, thread.index + Int3(0, 0,  1), 1) -
                                                 Helpers::getStaggeredVectorAtCell(velocityField, thread.index + Int3(0, 0, -1), 1));
            const float dux_dz = oneOverDx[2] * (Helpers::getStaggeredVectorAtCell(velocityField, thread.index + Int3(0, 0,  1), 0) -
                                                 Helpers::getStaggeredVectorAtCell(velocityField, thread.index + Int3(0, 0, -1), 0));
            const float duz_dx = oneOverDx[0] * (Helpers::getStaggeredVectorAtCell(velocityField, thread.index + Int3( 1, 0, 0), 2) -
                                                 Helpers::getStaggeredVectorAtCell(velocityField, thread.index + Int3(-1, 0, 0), 2));
            const float duy_dx = oneOverDx[0] * (Helpers::getStaggeredVectorAtCell(velocityField, thread.index + Int3( 1, 0, 0), 1) -
                                                 Helpers::getStaggeredVectorAtCell(velocityField, thread.index + Int3(-1, 0, 0), 1));
            const float dux_dy = oneOverDx[1] * (Helpers::getStaggeredVectorAtCell(velocityField, thread.index + Int3(0,  1, 0), 0) -
                                                 Helpers::getStaggeredVectorAtCell(velocityField, thread.index + Int3(0, -1, 0), 0));
            const Coords w = 0.5f * Coords(duz_dy - duy_dz, dux_dz - duz_dx, duy_dx - dux_dy);

            vorticityField[0].setValue(thread.index, w.x);
            vorticityField[1].setValue(thread.index, w.y);
            vorticityField[2].setValue(thread.index, w.z);
            vorticityNormField.setValue(thread.index, length(w));
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUIDS_VORTICITY_HPP */
