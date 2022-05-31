#ifndef HF_SIMULATOR_UTILITY_DOUBLEBUFFER_HPP
#define HF_SIMULATOR_UTILITY_DOUBLEBUFFER_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename _Type>
    struct DoubleBuffer
    {
    public:
        using Type = _Type;

    public:
        DoubleBuffer()
            : _current(0)
        {
        }

    public:
        Type& getFront()
        {
            return _objects[_current];
        }

        const Type& getFront() const
        {
            return _objects[_current];
        }

        Type& getBack()
        {
            return _objects[1 - _current];
        }

        const Type& getBack() const
        {
            return _objects[1 - _current];
        }

        void swap()
        {
            _current = 1 - _current;
        }

    public:
        Type* operator->()
        {
            return &(getFront());
        }

        const Type* operator->() const
        {
            return &(getFront());
        }
        
        Type& operator*()
        {
            return getFront();
        }
        
        const Type& operator*() const
        {
            return getFront();
        }
        
        Type& operator[](uint i)
        {
            return _objects[i];
        }
        
        const Type& operator[](uint i) const
        {
            return _objects[i];
        }

    private:
        Type _objects[2];
        uint _current;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF)

#endif /* HF_SIMULATOR_UTILITY_DOUBLEBUFFER_HPP */
