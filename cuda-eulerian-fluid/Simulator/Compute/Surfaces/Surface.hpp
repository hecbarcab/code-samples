#ifndef HF_SIMULATOR_COMPUTE_SURFACE_HPP
#define HF_SIMULATOR_COMPUTE_SURFACE_HPP

#include <Simulator/Simulator.hpp>
#include <Simulator/Compute/Location.hpp>
#include <Simulator/Compute/Arrays/Array.hpp>
#include <Simulator/Compute/Surfaces/SurfaceResourceType.hpp>
#include <Simulator/Compute/Surfaces/SurfaceBoundaryMode.hpp>
#include <Simulator/Compute/Surfaces/SurfaceAccessor.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order, typename _Value>
    class Surface
    {
    public:
        static constexpr uint Order = _Order;
        
        using LocationTag = Location::DeviceTag;
        static constexpr LocationTag Location = Location::Device;

        using Ref           = Ref<Surface>;
        using Value         = _Value;
        using BuiltInValue  = typename Traits<Value>::BuiltInType;
        using Dims          = IntN<Order>;
        using Index         = IntN<Order>;
        using Accessor      = SurfaceAccessor<Surface>;
        using ConstAccessor = SurfaceConstAccessor<Surface>;

    protected:
        Surface(const ArrayRef<Order, Value>& array, SurfaceBoundaryMode boundaryMode)
            : _dims(array->getDims())
            , _resource(array->getArray())
            , _resourceType(SurfaceResourceType::Array)
            , _boundaryMode(boundaryMode)
            , _surface(0)
        {
            // Allocate resource descriptor pointing to the given array.

            cudaResourceDesc resDesc;
            memset(&resDesc, 0, sizeof(cudaResourceDesc));
            resDesc.resType = static_cast<cudaResourceType>(SurfaceResourceType::Array);
            resDesc.res.array.array = array->getArray();

            // Create surface object.

            HF_CUDA(CreateSurfaceObject(&_surface, &resDesc));
        }

    public:

        Surface() = delete;

        ~Surface()
        {
            HF_IGNORE_THROW(HF_CUDA(DestroySurfaceObject(_surface)));
            _surface = 0;
        }

        HF_COPY_IMPLEMENTATION(Surface, delete)

        HF_MOVE_IMPLEMENTATION(Surface, default)

    public:
        static Ref Create(const ArrayRef<Order, Value>& array, SurfaceBoundaryMode boundaryMode)
        {
            return Ref(new Surface(array, boundaryMode));
        }

    public:

        /**
         * \brief 
         * \return 
         */
        bool isValid() const
        {
            return _surface != 0;
        }

        /**
         * \brief
         * \return
         */
        Dims getDims() const
        {
            return _dims;
        }

        /**
         * \brief 
         * \return 
         */
        void* getResource() const
        {
            return _resource;
        }

        /**
         * \brief 
         * \return 
         */
        SurfaceResourceType getSurfaceResourceType() const
        {
            return _resourceType;
        }

        /**
         * \brief 
         * \return 
         */
        SurfaceBoundaryMode getBoundaryMode() const
        {
            return _boundaryMode;
        }

        /**
         * \brief
         * \return
         */
        cudaSurfaceObject_t getSurfaceObject() const
        {
            return _surface;
        }

        /**
         * \brief
         * \return
         */
        ConstAccessor getConstAccessor() const
        {
            return ConstAccessor(_surface, _boundaryMode);
        }

        /**
         * \brief
         * \return
         */
        Accessor getAccessor()
        {
            return Accessor(_surface, _boundaryMode);
        }

    private:
        Dims                _dims;
        void*               _resource;
        SurfaceResourceType _resourceType;
        SurfaceBoundaryMode _boundaryMode;
        cudaSurfaceObject_t _surface;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint O, typename T> using SurfaceRef = typename Surface<O, T>::Ref;

    template <typename T> using Surface1  = Surface<1, T>;
    template <typename T> using Surface2  = Surface<2, T>;
    template <typename T> using Surface3  = Surface<3, T>;
    template <typename T> using Surface1Ref = typename Surface1<T>::Ref;
    template <typename T> using Surface2Ref = typename Surface2<T>::Ref;
    template <typename T> using Surface3Ref = typename Surface3<T>::Ref;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_SURFACE_HPP */

