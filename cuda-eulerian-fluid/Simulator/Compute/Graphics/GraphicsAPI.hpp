#ifndef HF_SIMULATOR_GRAPHICS_API_HPP
#define HF_SIMULATOR_GRAPHICS_API_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct GraphicsAPI
    {
        OpenGL,
        Direct3D9,
        Direct3D10,
        Direct3D11,
        Vulkan
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_GRAPHICS_API_HPP */
