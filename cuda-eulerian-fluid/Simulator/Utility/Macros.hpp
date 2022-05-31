#ifndef HF_SIMULATOR_UTILITY_MACROS_HPP
#define HF_SIMULATOR_UTILITY_MACROS_HPP

//─────────────────────────────────────────────────────────────────────────────────────────────────

#define HF_COPY_IMPLEMENTATION(Class, Kind)                                                       \
    Class(const Class&) = Kind;                                                                   \
    Class& operator=(const Class&) = Kind;

//─────────────────────────────────────────────────────────────────────────────────────────────────

#define HF_MOVE_IMPLEMENTATION(Class, Kind)                                                       \
    Class(Class&&) = Kind;                                                                        \
    Class& operator=(Class&&) = Kind;

//─────────────────────────────────────────────────────────────────────────────────────────────────

#define HF_HINLINE  __host__ inline
#define HF_DINLINE  __forceinline__ __device__
#define HF_HDINLINE __forceinline__ __host__ __device__

//─────────────────────────────────────────────────────────────────────────────────────────────────

#define HF_THREAD_IDX_1                                                                           \
    Int1(blockIdx.x * blockDim.x + threadIdx.x)

#define HF_THREAD_IDX_2                                                                           \
    Int2(blockIdx.x * blockDim.x + threadIdx.x,                                                   \
         blockIdx.y * blockDim.y + threadIdx.y)

#define HF_THREAD_IDX_3                                                                           \
    Int3(blockIdx.x * blockDim.x + threadIdx.x,                                                   \
         blockIdx.y * blockDim.y + threadIdx.y,                                                   \
         blockIdx.z * blockDim.z + threadIdx.z)

//─────────────────────────────────────────────────────────────────────────────────────────────────

#define HF_LOCAL_THREAD_IDX_1 Int1(threadIdx.x)
#define HF_LOCAL_THREAD_IDX_2 Int2(threadIdx.x, threadIdx.y)
#define HF_LOCAL_THREAD_IDX_3 Int3(threadIdx.x, threadIdx.y, threadIdx.z)

//─────────────────────────────────────────────────────────────────────────────────────────────────

#define HF_BLOCK_IDX_1 Int1(blockIdx.x)
#define HF_BLOCK_IDX_2 Int2(blockIdx.x, blockIdx.y)
#define HF_BLOCK_IDX_3 Int3(blockIdx.x, blockIdx.y, blockIdx.z)

//─────────────────────────────────────────────────────────────────────────────────────────────────

#define HF_BLOCK_DIMS_1 Int1(blockDim.x)
#define HF_BLOCK_DIMS_2 Int2(blockDim.x, blockDim.y)
#define HF_BLOCK_DIMS_3 Int3(blockDim.x, blockDim.y, blockDim.z)

//─────────────────────────────────────────────────────────────────────────────────────────────────

#define HF_STRINGIFY(A) #A

//─────────────────────────────────────────────────────────────────────────────────────────────────

#define HF_PREPROC_CONCAT_NOEXP(A, B) A ## B
#define HF_PREPROC_CONCAT(A, B)       HF_PREPROC_CONCAT_NOEXP(A, B)

//─────────────────────────────────────────────────────────────────────────────────────────────────

// Source: http://stackoverflow.com/questions/11358425/is-there-a-better-way-to-express-nested-namespaces-in-c-within-the-header
// Helper macros for variadic macro overloading.

#define HF_VA_HELPER_EXPAND(_X)                                 _X
#define HF_VA_COUNT_HELPER(_1, _2, _3, _4, _5, _6, _Count, ...) _Count
#define HF_VA_COUNT(...)                                        HF_VA_HELPER_EXPAND(HF_VA_COUNT_HELPER(__VA_ARGS__, 6, 5, 4, 3, 2, 1))
#define HF_VA_SELECT_CAT(_Name, _Count, ...)                    HF_VA_HELPER_EXPAND(_Name##_Count(__VA_ARGS__))
#define HF_VA_SELECT_HELPER(_Name, _Count, ...)                 HF_VA_SELECT_CAT(_Name, _Count, __VA_ARGS__)
#define HF_VA_SELECT(_Name, ...)                                HF_VA_SELECT_HELPER(_Name, HF_VA_COUNT(__VA_ARGS__), __VA_ARGS__)

#define HF_NAMESPACE_BEGIN_HELPER1(_Ns1)                        namespace _Ns1
#define HF_NAMESPACE_BEGIN_HELPER2(_Ns1, _Ns2)                  namespace _Ns1 { HF_NAMESPACE_BEGIN_HELPER1(_Ns2)
#define HF_NAMESPACE_BEGIN_HELPER3(_Ns1, _Ns2, _Ns3)            namespace _Ns1 { HF_NAMESPACE_BEGIN_HELPER2(_Ns2, _Ns3)

#define HF_NAMESPACE_END_HELPER1(_Ns1)  
#define HF_NAMESPACE_END_HELPER2(_Ns1, _Ns2) }                  HF_NAMESPACE_END_HELPER1(_Ns2)
#define HF_NAMESPACE_END_HELPER3(_Ns1, _Ns2, _Ns3) }            HF_NAMESPACE_END_HELPER2(_Ns2, _Ns3)

#define HF_BEGIN_NAMESPACE(_Namespace, ...)                     HF_VA_SELECT(HF_NAMESPACE_BEGIN_HELPER, _Namespace, __VA_ARGS__)
#define HF_END_NAMESPACE(_Namespace, ...)                       HF_VA_SELECT(HF_NAMESPACE_END_HELPER,   _Namespace, __VA_ARGS__)

//─────────────────────────────────────────────────────────────────────────────────────────────────

#endif /* HF_SIMULATOR_UTILITY_MACROS_HPP */
