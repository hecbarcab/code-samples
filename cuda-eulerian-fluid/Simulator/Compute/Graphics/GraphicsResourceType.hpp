#ifndef HF_SIMULATOR_GRAPHICS_RESOURCE_TYPE_HPP
#define HF_SIMULATOR_GRAPHICS_RESOURCE_TYPE_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct GraphicsResourceType
    {
        Texture,
        Buffer
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_GRAPHICS_RESOURCE_TYPE_HPP */
