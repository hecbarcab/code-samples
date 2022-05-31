#ifndef HF_SIMULATOR_UTILITY_TRIPLEBUFFER_HPP
#define HF_SIMULATOR_UTILITY_TRIPLEBUFFER_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename _Type>
    struct TripleBuffer
    {
    public:
        using Type = _Type;

    public:
        TripleBuffer()
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

        Type& getMiddle()
        {
            return _objects[(_current + 1) % 3];
        }

        const Type& getMiddle() const
        {
            return _objects[(_current + 1) % 3];
        }

        Type& getBack()
        {
            return _objects[(_current + 2) % 3];
        }

        const Type& getBack() const
        {
            return _objects[(_current + 2) % 3];
        }

        void swap()
        {
            _current = (_current + 1) % 3;
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
        Type _objects[3];
        uint _current;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF)

#endif /* HF_SIMULATOR_UTILITY_TRIPLEBUFFER_HPP */
