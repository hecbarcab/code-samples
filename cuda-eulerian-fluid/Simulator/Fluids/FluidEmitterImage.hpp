#ifndef HF_SIMULATION_FLUID_EMITTER_IMAGE_HPP
#define HF_SIMULATION_FLUID_EMITTER_IMAGE_HPP

#include <Simulator/Compute/Kernel.hpp>
#include <Simulator/Fluids/Fluid.hpp>
#include <Simulator/Fluids/Kernels/EmissionImageKernel.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order>
    class FluidEmitterImage
    {
        static_assert(_Order >= 2 && _Order <= 3, "Only 2D and 3D fluid emitters are supported.");

    public:
        static constexpr uint Order = _Order;

        using Fluid    = Fluid<Order>;
        using FluidRef = typename Fluid::Ref;
        using Index    = IntN<Order>;

    public:
        FluidEmitterImage() = default;

        FluidEmitterImage(const Index& offset, int width, int height, int extrusion, uchar* data)
            : _offset(offset)
            , _size(width, height)
            , _extrusion(extrusion)
        {
            _image = FluidScalarField2<Uchar4>::Create(Int2(width, height));

            auto accessor = _image->getAccessor(Compute::Location::Host);
            std::memcpy(reinterpret_cast<void*>(accessor.getPtr()),
                        reinterpret_cast<void*>(data),
                        sizeof(uchar) * 4 * width * height);

            _image->copyHostToDevice();
        }

    public:
        template <typename LocationTag>
        void emit(const FluidRef& fluid, const LocationTag& location) const
        {
            // FIXME: Assumes 3D fluid,

            const auto threadCount = Int3(_size, _extrusion);

            Compute::Kernel::execute<Order, EmissionImageKernel>(location,
                                                                 threadCount,
                                                                fluid->getInkField()->getAccessor(location),
                                                                _offset,
                                                                _size,
                                                                _extrusion,
                                                                _image->getConstAccessor(location));
        }

    private:
        Index                        _offset;
        Int2                         _size;
        int                          _extrusion;
        FluidScalarField2Ref<Uchar4> _image;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using FluidEmitterImage2 = FluidEmitterImage<2>;
    using FluidEmitterImage3 = FluidEmitterImage<3>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATION_FLUID_EMITTER_IMAGE_HPP */
