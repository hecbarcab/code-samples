#ifndef HF_SIMULATOR_COMPUTE_BUFFER_ACCESSOR_HPP
#define HF_SIMULATOR_COMPUTE_BUFFER_ACCESSOR_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename _Buffer>
    struct BufferAccessor
    {
        friend _Buffer;

    public:
        static constexpr uint Order = _Buffer::Order;
        
        using Buffer  = _Buffer;
        using Value   = typename _Buffer::Value;
        using Index   = typename _Buffer::Index;
        using Indexer = typename _Buffer::Indexer;

    public:
        BufferAccessor() = default;

        BufferAccessor(const Indexer& indexer, Value* ptr)
            : _indexer(indexer)
            , _ptr(ptr)
        {
        }

        HF_COPY_IMPLEMENTATION(BufferAccessor, default)

        HF_MOVE_IMPLEMENTATION(BufferAccessor, default)


    public:
        /**
         * \brief 
         * \param idx 
         * \return 
         */
        HF_HDINLINE Value getValue(const Index& idx) const
        {
            return _ptr[_indexer.computeOffset(idx)];
        }

        /**
         * \brief 
         * \param idx 
         * \param value 
         */
        HF_HDINLINE void setValue(const Index& idx, const Value& value)
        {
            _ptr[_indexer.computeOffset(idx)] = value;
        }

        /**
         * \brief
         * \return
         */
        HF_HDINLINE Value* getPtr()
        {
            return _ptr;
        }

        /**
         * \brief
         * \return
         */
        HF_HDINLINE Value const* getPtr() const
        {
            return _ptr;
        }

        /**
         * \brief
         * \return
         */
        HF_HDINLINE Value const* getPtr(const Index& idx) const
        {
            return &_ptr[_indexer.computeOffset(idx)];
        }

    private:
        Indexer _indexer;
        Value*  _ptr;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename _Buffer>
    struct BufferConstAccessor
    {
        friend _Buffer;

    public:
        static constexpr uint Order = _Buffer::Order;

        using Buffer  = _Buffer;
        using Value   = typename _Buffer::Value;
        using Index   = typename _Buffer::Index;
        using Indexer = typename _Buffer::Indexer;
        

    public:
        BufferConstAccessor() = default;

        BufferConstAccessor(const Indexer& indexer, Value* ptr)
            : _indexer(indexer)
            , _ptr(ptr)
        {
        }

        HF_COPY_IMPLEMENTATION(BufferConstAccessor, default)

        HF_MOVE_IMPLEMENTATION(BufferConstAccessor, default)

    public:
        /**
         * \brief
         * \param idx
         * \return
         */
        HF_HDINLINE Value getValue(const Index& idx) const
        {
            return _ptr[_indexer.computeOffset(idx)];
        }

        /**
         * \brief
         * \return
         */
        HF_HDINLINE Value* getPtr() const
        {
            return _ptr;
        }

        /**
         * \brief
         * \return
         */
        HF_HDINLINE Value* getPtr(const Index& idx) const
        {
            return &_ptr[_indexer.computeOffset(idx)];
        }

    private:
        Indexer _indexer;
        Value*  _ptr;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_BUFFER_ACCESSOR_HPP */
