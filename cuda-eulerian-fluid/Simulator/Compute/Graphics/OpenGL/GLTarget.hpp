#ifndef HF_SIMULATOR_GRAPHICS_OPENGL_TARGET_HPP
#define HF_SIMULATOR_GRAPHICS_OPENGL_TARGET_HPP

#include <Simulator/Simulator.hpp>

#if HF_SUPPORT_OPENGL_INTEROP == 1

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct GLTarget
    {
        Texture2D      = GL_TEXTURE_2D,
        Texture3D      = GL_TEXTURE_3D,
        Texture2DArray = GL_TEXTURE_2D_ARRAY,
        TextureRect    = GL_TEXTURE_RECTANGLE,
        TextureCubeMap = GL_TEXTURE_CUBE_MAP,
        RenderBuffer   = GL_RENDERBUFFER
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, COMPUTE)

#endif /* HF_SUPPORT_OPENGL_INTEROP */
#endif /* HF_SIMULATOR_GRAPHICS_OPENGL_TARGET_HPP */