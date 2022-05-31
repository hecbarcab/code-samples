#ifndef HF_SIMULATOR_COMPUTE_TEXTURE_ADDRESS_MODE_HPP
#define HF_SIMULATOR_COMPUTE_TEXTURE_ADDRESS_MODE_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct TextureAddressMode
    {
        Wrap   = cudaAddressModeWrap,
        Clamp  = cudaAddressModeClamp,
        Mirror = cudaAddressModeMirror,
        Border = cudaAddressModeBorder
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_TEXTURE_ADDRESS_MODE_HPP */
