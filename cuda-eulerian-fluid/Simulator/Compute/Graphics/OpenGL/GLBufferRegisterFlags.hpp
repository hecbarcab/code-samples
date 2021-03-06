#ifndef HF_SIMULATOR_GRAPHICS_OPENGL_BUFFER_REGISTER_FLAGS_HPP
#define HF_SIMULATOR_GRAPHICS_OPENGL_BUFFER_REGISTER_FLAGS_HPP

#include <Simulator/Simulator.hpp>

#if HF_SUPPORT_OPENGL_INTEROP == 1

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    enum struct GLBufferRegisterFlags
    {
        None         = cudaGraphicsRegisterFlagsNone,
        ReadOnly     = cudaGraphicsRegisterFlagsReadOnly,
        WriteDiscard = cudaGraphicsRegisterFlagsWriteDiscard
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, COMPUTE)

#endif /* HF_SUPPORT_OPENGL_INTEROP */
#endif /* HF_SIMULATOR_GRAPHICS_OPENGL_BUFFER_REGISTER_FLAGS_HPP */