#ifndef HF_SIMULATOR_UTILITY_TYPES_HPP
#define HF_SIMULATOR_UTILITY_TYPES_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    // Common primitive types.

    using uchar     = unsigned char;
    using ushort    = unsigned short;
    using uint      = unsigned int;
    using ulong     = unsigned long;
    using ulonglong = unsigned long long;

    //─────────────────────────────────────────────────────────────────────────────────────────────

    // Math types

    template <typename Scalar, uint Length>                 using VectorN  = glm::vec<Length, Scalar>;
    template <typename Scalar, uint Rows, uint Cols = Rows> using MatrixNM = glm::mat<Cols, Rows, Scalar>;

    template <uint Length> using BoolN  = VectorN<bool,  Length>;
    template <uint Length> using UcharN = VectorN<uchar, Length>;
    template <uint Length> using IntN   = VectorN<int,   Length>;
    template <uint Length> using UintN  = VectorN<uint,  Length>;
    template <uint Length> using FloatN = VectorN<float, Length>;

    template <uint Rows, uint Cols = Rows> using BoolNM  = MatrixNM<bool,  Rows, Cols>;
    template <uint Rows, uint Cols = Rows> using UcharNM = MatrixNM<uchar, Rows, Cols>;
    template <uint Rows, uint Cols = Rows> using IntNM   = MatrixNM<int,   Rows, Cols>;
    template <uint Rows, uint Cols = Rows> using UintNM  = MatrixNM<uint,  Rows, Cols>;
    template <uint Rows, uint Cols = Rows> using FloatNM = MatrixNM<float, Rows, Cols>;

    // Aliases for common types

    using Bool1      = BoolN<1>;
    using Bool2      = BoolN<2>;
    using Bool3      = BoolN<3>;
    using Bool4      = BoolN<4>;
    using Uchar1     = UcharN<1>;
    using Uchar2     = UcharN<2>;
    using Uchar3     = UcharN<3>;
    using Uchar4     = UcharN<4>;
    using Int1       = IntN<1>;
    using Int2       = IntN<2>;
    using Int3       = IntN<3>;
    using Int4       = IntN<4>;
    using Uint1      = UintN<1>;
    using Uint2      = UintN<2>;
    using Uint3      = UintN<3>;
    using Uint4      = UintN<4>;
    using Float1     = FloatN<1>;
    using Float2     = FloatN<2>;
    using Float3     = FloatN<3>;
    using Float4     = FloatN<4>;
    using Float2x2   = FloatNM<2, 2>;
    using Float3x3   = FloatNM<3, 3>;
    using Float4x4   = FloatNM<4, 4>;
    using Float2x3   = FloatNM<2, 3>;
    using Float2x4   = FloatNM<2, 4>;
    using Float3x2   = FloatNM<3, 2>;
    using Float3x4   = FloatNM<3, 4>;
    using Float4x2   = FloatNM<4, 2>;
    using Float4x3   = FloatNM<4, 3>;
    using Quaternion = glm::quat;

    // Traits for common types

    template <typename Vec> struct Traits;

    #define HF_DEFINE_TRAITS(T, S, D, FP, B)                                                      \
        template <> struct Traits<T>                                                              \
        {                                                                                         \
            using ScalarType = S;                                                                 \
            static constexpr uint Dims = D;                                                       \
            static constexpr bool IsFloatingPoint = FP;                                           \
            using BuiltInType = B;                                                                \
        }                            
    
    HF_DEFINE_TRAITS(bool,   bool,  1, false, uchar1);
    HF_DEFINE_TRAITS(Bool1,  bool,  1, false, uchar1);
    HF_DEFINE_TRAITS(Bool2,  bool,  2, false, uchar2);
    HF_DEFINE_TRAITS(Bool3,  bool,  3, false, uchar3);
    HF_DEFINE_TRAITS(Bool4,  bool,  4, false, uchar4);
    HF_DEFINE_TRAITS(uchar,  bool,  1, false, uchar1);
    HF_DEFINE_TRAITS(Uchar1, bool,  1, false, uchar1);
    HF_DEFINE_TRAITS(Uchar2, bool,  2, false, uchar2);
    HF_DEFINE_TRAITS(Uchar3, bool,  3, false, uchar3);
    HF_DEFINE_TRAITS(Uchar4, bool,  4, false, uchar4);
    HF_DEFINE_TRAITS(uint,   uint,  1, false, uint1);
    HF_DEFINE_TRAITS(Uint1,  uint,  1, false, uint1);
    HF_DEFINE_TRAITS(Uint2,  uint,  2, false, uint2);
    HF_DEFINE_TRAITS(Uint3,  uint,  3, false, uint3);
    HF_DEFINE_TRAITS(Uint4,  uint,  4, false, uint4);
    HF_DEFINE_TRAITS(int,    int,   1, false, int1);
    HF_DEFINE_TRAITS(Int1,   int,   1, false, int1);
    HF_DEFINE_TRAITS(Int2,   int,   2, false, int2);
    HF_DEFINE_TRAITS(Int3,   int,   3, false, int3);
    HF_DEFINE_TRAITS(Int4,   int,   4, false, int4);
    HF_DEFINE_TRAITS(float,  float, 1, true,  float1);
    HF_DEFINE_TRAITS(Float1, float, 1, true,  float1);
    HF_DEFINE_TRAITS(Float2, float, 2, true,  float2);
    HF_DEFINE_TRAITS(Float3, float, 3, true,  float3);
    HF_DEFINE_TRAITS(Float4, float, 4, true,  float4);

    #undef HF_DEFINE_TRAITS

    template <typename T> struct ScalarOf
    {
        using Type = typename Traits<T>::ScalarType;
    };

    template <typename T> struct DimsOf
    {
        static constexpr uint Value = Traits<T>::Dims;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    // Aliases to common std::types.

    template <typename T1, typename T2> using Pair = std::pair<T1, T2>;

    // Ref-counting pointers

    template <typename T> using Ref       = std::shared_ptr<T>;
    template <typename T> using WeakRef   = std::weak_ptr<T>;
    template <typename T> using UniqueRef = std::unique_ptr<T>;

    // Hash for common std::types.

    struct PairHash
    {
    public:
        template <typename T1, typename T2>
        std::size_t operator()(const std::pair<T1, T2> &x) const
        {
            return std::hash<T1>()(x.first) ^ std::hash<T2>()(x.second);
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF)

#endif /* HF_SIMULATOR_UTILITY_TYPES_HPP */
