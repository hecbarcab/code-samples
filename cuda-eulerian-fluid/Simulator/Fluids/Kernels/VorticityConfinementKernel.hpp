#ifndef HF_SIMULATOR_FLUIDS_VORTICITY_CONFINEMENT_HPP
#define HF_SIMULATOR_FLUIDS_VORTICITY_CONFINEMENT_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/KernelConfig.hpp>
#include <Simulator/Compute/KernelThread.hpp>
#include <Simulator/Compute/KernelBlockDims.hpp>
#include <Simulator/Fluids/FluidDomain.hpp>
#include <Simulator/Fluids/Kernels/Helpers.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint Order, typename LocationTag>
    struct VorticityConfinementKernel;

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename LocationTag>
    struct VorticityConfinementKernel<3, LocationTag>
    {
        using Config = Compute::KernelConfig<Compute::KernelBlockDims::Inferred>;
                      
        using Thread  = Compute::KernelThread3;
        using Domain  = FluidDomain3;
        using Coords  = Float3;
        using Index   = Int3;
        using Helpers = Helpers<3>;

        template <typename VorticityConstAccessor, 
                  typename VorticityNormConstAccessor, 
                  typename ConfinementAccessor>
        static HF_HDINLINE void kernel(Thread                     thread,
                                       Domain                     dom,
                                       VorticityConstAccessor     vorticityField,
                                       VorticityNormConstAccessor vorticityNormField,
                                       ConfinementAccessor        confinementField,
                                       float                      epsilon)
        {
            if (any(greaterThanEqual(thread.index, dom.getDims())))
                return;

            const Coords& dx = dom.getDx();

            // Compute vorticity confinement at the center of the cell
            // First compute N = eta / |eta|, where eta = ∇|w|

            const float wxf = vorticityNormField.getValue(thread.index + Index(1, 0, 0));
            const float wxb = vorticityNormField.getValue(thread.index + Index(-1, 0, 0));
            const float wyf = vorticityNormField.getValue(thread.index + Index(0, 1, 0));
            const float wyb = vorticityNormField.getValue(thread.index + Index(0, -1, 0));
            const float wzf = vorticityNormField.getValue(thread.index + Index(0, 0, 1));
            const float wzb = vorticityNormField.getValue(thread.index + Index(0, 0, -1));

            const Coords eta = 0.5f * Coords(wxf - wxb, wyf - wyb, wzf - wzb) / dx;
            const Coords N = eta / (length(eta) + 1e-5f);

            // Then compute confinement force
            // f = eps h (N×w)

            const Coords w = Helpers::getVectorAtCell(vorticityField, thread.index);

            const Coords f = epsilon * dx * cross(N, w);

            confinementField[0].setValue(thread.index, f.x);
            confinementField[1].setValue(thread.index, f.y);
            confinementField[2].setValue(thread.index, f.z);
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUIDS_VORTICITY_CONFINEMENT_HPP */
