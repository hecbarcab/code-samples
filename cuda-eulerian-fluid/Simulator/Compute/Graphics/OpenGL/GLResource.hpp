#ifndef HF_COMPUTE_GRAPHICS_OPENGL_RESOURCE_HPP
#define HF_COMPUTE_GRAPHICS_OPENGL_RESOURCE_HPP

#include <Simulator/Simulator.hpp>

#if HF_SUPPORT_OPENGL_INTEROP == 1
#include <Simulator/Compute/Graphics/GraphicsAPI.hpp>
#include <Simulator/Compute/Graphics/GraphicsResource.hpp>
#include <Simulator/Compute/Graphics/GraphicsResourceType.hpp>
#include <Simulator/Compute/Graphics/OpenGL/GLBufferRegisterFlags.hpp>
#include <Simulator/Compute/Graphics/OpenGL/GLImageRegisterFlags.hpp>
#include <Simulator/Compute/Graphics/OpenGL/GLTarget.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <>
    class GraphicsResource<GraphicsAPI::OpenGL>
    {
    public:
        static constexpr GraphicsAPI API = GraphicsAPI::OpenGL;
        using Ref = Ref<GraphicsResource>;

    public:
        GLuint getGLHandle() const
        {
            return _handle;
        }

        cudaGraphicsResource_t getResource() const
        {
            return _resource;
        }

        GraphicsResourceType getType() const
        {
            return _type;
        }

    protected:
        GraphicsResource(GLuint handle, GLBufferRegisterFlags flags)
            : _type(GraphicsResourceType::Buffer)
            , _handle(handle)
            , _resource(nullptr)
        {
            HF_CUDA(GraphicsGLRegisterBuffer(&_resource, _handle, static_cast<unsigned>(flags)));
        }

        GraphicsResource(GLuint handle, GLTarget target, GLImageRegisterFlags flags)
            : _type(GraphicsResourceType::Texture)
            , _handle(handle)
            , _resource(nullptr)
        {
            HF_CUDA(GraphicsGLRegisterImage(&_resource, _handle, 
                                            static_cast<GLenum>(target), 
                                            static_cast<unsigned>(flags)));
        }

    public:
        HF_COPY_IMPLEMENTATION(GraphicsResource, delete)

        HF_MOVE_IMPLEMENTATION(GraphicsResource, default)
    
        ~GraphicsResource()
        {
            if (_resource == nullptr) return;
            HF_HALT_ON_THROW(HF_CUDA(GraphicsUnregisterResource(_resource)));
        }

    public:
        GraphicsResourceType   _type;
        GLuint                 _handle;
        cudaGraphicsResource_t _resource;

    public:
        static Ref Create(GLuint handle, GLBufferRegisterFlags flags)
        {
            return Ref(new GraphicsResource(handle, flags));
        }

        static Ref Create(GLuint handle, GLTarget target, GLImageRegisterFlags flags)
        {
            return Ref(new GraphicsResource(handle, target, flags));
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    using GLResource    = GraphicsResource<GraphicsAPI::OpenGL>;
    using GLResourceRef = typename GLResource::Ref;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SUPPORT_OPENGL_INTEROP */
#endif /* HF_COMPUTE_GRAPHICS_OPENGL_OPENGLRESOURCE_HPP */