#ifndef HF_SIMULATOR_FLUID_VECTOR_FIELD_HPP
#define HF_SIMULATOR_FLUID_VECTOR_FIELD_HPP

#include <Simulator/Fluids/FluidDomain.hpp>
#include <Simulator/Fluids/FluidScalarField.hpp>
#include <Simulator/Utility/Containers/FixedArray.hpp>

HF_BEGIN_NAMESPACE(HF, Simulator)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint _Order, typename _Value>
    class FluidVectorField
    {
    public:
        static constexpr uint Order = _Order;

        using Ref      = Ref<FluidVectorField>;
        using Value    = _Value;
        using Domain   = FluidDomain<Order>;
        using Dims     = IntN<Order>;
        using Index    = IntN<Order>;
        using Field    = FluidScalarField<Order, Value>;
        using FieldRef = FluidScalarFieldRef<Order, Value>;

        using HostAccessor        = FixedArray<Order, typename Field::HostAccessor>;
        using HostConstAccessor   = FixedArray<Order, typename Field::HostConstAccessor>;
        using HostSampler         = FixedArray<Order, typename Field::HostSampler>;

        using DeviceAccessor      = FixedArray<Order, typename Field::DeviceAccessor>;
        using DeviceConstAccessor = FixedArray<Order, typename Field::DeviceConstAccessor>;
        using DeviceSampler       = FixedArray<Order, typename Field::DeviceSampler>;
        
    protected:
        FluidVectorField(const Dims& dims)
            : _staggered(false)
        {
            for (uint i = 0; i < Order; i++)
                _fields[i] = Field::Create(dims);
        }

        FluidVectorField(const Domain& domain, bool staggered)
            : _staggered(staggered)
        {
            if (staggered)
            {
                for (uint i = 0; i < Order; i++)
                    _fields[i] = Field::Create(domain.getDimsOfFaceGrid(i));
            }
            else
            {
                for (uint i = 0; i < Order; i++)
                    _fields[i] = Field::Create(domain.getDims());
            }
        }

    public:
        bool isStaggered() const
        {
            return _staggered;
        }

        const FieldRef& getAxis(uint axis) const
        {
            return _fields[axis];
        }

        HostConstAccessor getConstAccessor(const Compute::Location::HostTag& location) const
        {
            HostConstAccessor constAccessor;
            for (uint i = 0; i < Order; i++) constAccessor[i] = _fields[i]->getConstAccessor(location);
            return constAccessor;
        }

        HostAccessor getAccessor(const Compute::Location::HostTag& location)
        {
            HostAccessor accessor;
            for (uint i = 0; i < Order; i++) accessor[i] = _fields[i]->getAccessor(location);
            return accessor;
        }

        HostSampler getSampler(const Compute::Location::HostTag& location) const
        {
            HostSampler sampler;
            for (uint i = 0; i < Order; i++) sampler[i] = _fields[i]->getSampler(location);
            return sampler;
        }

        DeviceConstAccessor getConstAccessor(const Compute::Location::DeviceTag& location) const
        {
            DeviceConstAccessor constAccessor;
            for (uint i = 0; i < Order; i++) constAccessor[i] = _fields[i]->getConstAccessor(location);
            return constAccessor;
        }

        DeviceAccessor getAccessor(const Compute::Location::DeviceTag& location)
        {
            DeviceAccessor accessor;
            for (uint i = 0; i < Order; i++) accessor[i] = _fields[i]->getAccessor(location);
            return accessor;
        }

        DeviceSampler getSampler(const Compute::Location::DeviceTag& location) const
        {
            DeviceSampler sampler;
            for (uint i = 0; i < Order; i++) sampler[i] = _fields[i]->getSampler(location);
            return sampler;
        }

        void copyHostToDevice()
        {
            for (uint i = 0; i < Order; ++i)
                _fields[i]->copyHostToDevice();
        }

        void copyDeviceToHost()
        {
            for (uint i = 0; i < Order; ++i)
                _fields[i]->copyDeviceToHost();
        }

        template <typename Resource>
        void copyDeviceToResource(const Resource& res, uint index)
        {
            _fields[index]->copyDeviceToResource(res);
        }

        template <typename LocationTag>
        bool loadFromFile(const LocationTag& location, const std::string& filename, uint axis) const
        {
            return _fields[axis]->loadFromFile(location, filename);
        }

        template <typename LocationTag>
        bool saveToFile(const LocationTag& location, const std::string& filename, uint axis) const
        {
            return _fields[axis]->saveToFile(location, filename);
        }

    private:
        bool     _staggered;
        FieldRef _fields[Order];

    public:
        static Ref Create(const Dims& dims)
        {
            return Ref(new FluidVectorField(dims));
        }

        static Ref Create(const Domain& domain, bool staggered)
        {
            return Ref(new FluidVectorField(domain, staggered));
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <uint O, typename V> using FluidVectorFieldRef = typename FluidVectorField<O, V>::Ref;

    template <typename V> using FluidVectorField1 = FluidVectorField<1, V>;
    template <typename V> using FluidVectorField2 = FluidVectorField<2, V>;
    template <typename V> using FluidVectorField3 = FluidVectorField<3, V>;
    template <typename V> using FluidVectorField1Ref = FluidVectorFieldRef<1, V>;
    template <typename V> using FluidVectorField2Ref = FluidVectorFieldRef<2, V>;
    template <typename V> using FluidVectorField3Ref = FluidVectorFieldRef<3, V>;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Simulator)

#endif /* HF_SIMULATOR_FLUID_VECTOR_FIELD_HPP */
                        