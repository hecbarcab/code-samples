#ifndef HF_SIMULATOR_UTILITY_FUNCTIONS_HPP
#define HF_SIMULATOR_UTILITY_FUNCTIONS_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    // Common math functions

    using glm::abs;
    using glm::ceil;
    using glm::clamp;
    using glm::floatBitsToInt;
    using glm::floatBitsToUint;
    using glm::floor;
    using glm::fma;
    using glm::fract;
    using glm::frexp;
    using glm::intBitsToFloat;
    using glm::isinf;
    using glm::isnan;
    using glm::ldexp;
    using glm::max;
    using glm::min;
    using glm::mix;
    using glm::mod;
    using glm::mod;
    using glm::modf;
    using glm::round;
    using glm::roundEven;
    using glm::sign;
    using glm::smoothstep;
    using glm::step;
    using glm::trunc;
    using glm::uintBitsToFloat;

    // Exponential functions

    using glm::exp;
    using glm::exp2;
    using glm::inversesqrt;
    using glm::log;
    using glm::log2;
    using glm::pow;
    using glm::sqrt;

    // Geometric functions

    using glm::cross;
    using glm::distance;
    using glm::dot;
    using glm::faceforward;
    using glm::length;
    using glm::normalize;
    using glm::reflect;
    using glm::refract;

    // Integer functions

    using glm::bitCount;
    using glm::bitfieldExtract;
    using glm::bitfieldInsert;
    using glm::bitfieldReverse;
    using glm::findLSB;
    using glm::findMSB;
    using glm::imulExtended;
    using glm::uaddCarry;
    using glm::umulExtended;
    using glm::usubBorrow;

    // Matrix functions

    using glm::determinant;
    using glm::inverse;
    using glm::matrixCompMult;
    using glm::outerProduct;
    using glm::transpose;

    // Floating-Point Pack and Unpack Functions

    using glm::packDouble2x32;
    using glm::packHalf2x16;
    using glm::packSnorm2x16;
    using glm::packSnorm4x8;
    using glm::packUnorm2x16;
    using glm::packUnorm4x8;
    using glm::unpackDouble2x32;
    using glm::unpackHalf2x16;
    using glm::unpackSnorm2x16;
    using glm::unpackSnorm4x8;
    using glm::unpackUnorm2x16;
    using glm::unpackUnorm4x8;

    // Angle and Trigonometry Functions

    using glm::acos;
    using glm::acosh;
    using glm::asin;
    using glm::asinh;
    using glm::atan;
    using glm::atan;
    using glm::atanh;
    using glm::cos;
    using glm::cosh;
    using glm::degrees;
    using glm::radians;
    using glm::sin;
    using glm::sinh;
    using glm::tan;
    using glm::tanh;

    // Vector Relational Functions

    using glm::all;
    using glm::any;
    using glm::equal;
    using glm::greaterThan;
    using glm::greaterThanEqual;
    using glm::lessThan;
    using glm::lessThanEqual;
    using glm::notEqual;

    // (Ext) Component-wise functions

    using glm::compNormalize;
    using glm::compScale;
    using glm::compAdd;
    using glm::compMul;
    using glm::compMin;
    using glm::compMax;

    // (Ext) Transformation matrices functions

    using glm::frustum;
    using glm::infinitePerspective;
    using glm::lookAt;
    using glm::ortho;
    using glm::perspective;
    using glm::perspectiveFov;
    using glm::pickMatrix;
    using glm::project;
    using glm::rotate;
    using glm::scale;
    using glm::translate;
    using glm::tweakedInfinitePerspective;
    using glm::unProject;

    // (Ext) Quaternions

    using glm::toMat4;
    using glm::conjugate;
    using glm::angleAxis;
    using glm::quatLookAt;

    //─────────────────────────────────────────────────────────────────────────────────────────────

    // Casting to CUDA built-in types

    HF_HDINLINE uchar1 cast(const uchar &x)  { return make_uchar1(uchar(x)); }
    HF_HDINLINE uchar2 cast(const Uchar2 &v) { return make_uchar2(uchar(v.x), uchar(v.y)); }
    HF_HDINLINE uchar3 cast(const Uchar3 &v) { return make_uchar3(uchar(v.x), uchar(v.y), uchar(v.z)); }
    HF_HDINLINE uchar4 cast(const Uchar4 &v) { return make_uchar4(uchar(v.x), uchar(v.y), uchar(v.z), uchar(v.w)); }
    HF_HDINLINE int1   cast(const int &x)    { return make_int1(x); }
    HF_HDINLINE int2   cast(const Int2 &v)   { return make_int2(v.x, v.y); }
    HF_HDINLINE int3   cast(const Int3 &v)   { return make_int3(v.x, v.y, v.z); }
    HF_HDINLINE int4   cast(const Int4 &v)   { return make_int4(v.x, v.y, v.z, v.w); }
    HF_HDINLINE uint1  cast(const uint &x)   { return make_uint1(x); }
    HF_HDINLINE uint2  cast(const Uint2 &v)  { return make_uint2(v.x, v.y); }
    HF_HDINLINE uint3  cast(const Uint3 &v)  { return make_uint3(v.x, v.y, v.z); }
    HF_HDINLINE uint4  cast(const Uint4 &v)  { return make_uint4(v.x, v.y, v.z, v.w); }
    HF_HDINLINE float1 cast(const float &x)  { return make_float1(x); }
    HF_HDINLINE float2 cast(const Float2 &v) { return make_float2(v.x, v.y); }
    HF_HDINLINE float3 cast(const Float3 &v) { return make_float3(v.x, v.y, v.z); }
    HF_HDINLINE float4 cast(const Float4 &v) { return make_float4(v.x, v.y, v.z, v.w); }

    HF_HDINLINE uchar  cast(const uchar1 &v) { return v.x; }
    HF_HDINLINE Uchar2 cast(const uchar2 &v) { return Uchar2(v.x, v.y); }
    HF_HDINLINE Uchar3 cast(const uchar3 &v) { return Uchar3(v.x, v.y, v.z); }
    HF_HDINLINE Uchar4 cast(const uchar4 &v) { return Uchar4(v.x, v.y, v.z, v.w); }
    HF_HDINLINE int    cast(const int1 &v)   { return v.x; }
    HF_HDINLINE Int2   cast(const int2 &v)   { return Int2(v.x, v.y); }
    HF_HDINLINE Int3   cast(const int3 &v)   { return Int3(v.x, v.y, v.z); }
    HF_HDINLINE Int4   cast(const int4 &v)   { return Int4(v.x, v.y, v.z, v.w); }
    HF_HDINLINE uint   cast(const uint1 &v)  { return v.x; }
    HF_HDINLINE Uint2  cast(const uint2 &v)  { return Uint2(v.x, v.y); }
    HF_HDINLINE Uint3  cast(const uint3 &v)  { return Uint3(v.x, v.y, v.z); }
    HF_HDINLINE Uint4  cast(const uint4 &v)  { return Uint4(v.x, v.y, v.z, v.w); }
    HF_HDINLINE float  cast(const float1 &v) { return v.x; }
    HF_HDINLINE Float2 cast(const float2 &v) { return Float2(v.x, v.y); }
    HF_HDINLINE Float3 cast(const float3 &v) { return Float3(v.x, v.y, v.z); }
    HF_HDINLINE Float4 cast(const float4 &v) { return Float4(v.x, v.y, v.z, v.w); }

    HF_HINLINE dim3 castCudaDims(const Int1& v) { return { uint(v.x) }; }
    HF_HINLINE dim3 castCudaDims(const Int2& v) { return { uint(v.x), uint(v.y) }; }
    HF_HINLINE dim3 castCudaDims(const Int3& v) { return { uint(v.x), uint(v.y), uint(v.z) }; }

    HF_HINLINE cudaPos castCudaPos(const Int1& v) { return make_cudaPos(uint(v.x), 0, 0); }
    HF_HINLINE cudaPos castCudaPos(const Int2& v) { return make_cudaPos(uint(v.x), uint(v.y), 0); }
    HF_HINLINE cudaPos castCudaPos(const Int3& v) { return make_cudaPos(uint(v.x), uint(v.y), uint(v.z)); }

    HF_HINLINE cudaExtent castCudaExtents(const Int1& v) { return make_cudaExtent(uint(v.x), 1, 1); }
    HF_HINLINE cudaExtent castCudaExtents(const Int2& v) { return make_cudaExtent(uint(v.x), uint(v.y), 1); }
    HF_HINLINE cudaExtent castCudaExtents(const Int3& v) { return make_cudaExtent(uint(v.x), uint(v.y), uint(v.z)); }

    //─────────────────────────────────────────────────────────────────────────────────────────────

    // Additional math functions

    template <typename Vec, typename Scalar = typename ScalarOf<Vec>::Type, uint Dims = DimsOf<Vec>::Value>
    HF_HDINLINE Vec exclusiveCumSum(const Vec &v)
    {
        Vec cumSum;
        cumSum[0] = Scalar(0);

        for (uint i = 1; i < Dims; ++i) 
            cumSum[i] = cumSum[i - 1] + v[i - 1];

        return cumSum;
    }

    template <typename Vec, typename Scalar = typename ScalarOf<Vec>::Type, uint Dims = DimsOf<Vec>::Value>
    HF_HDINLINE Vec inclusiveCumSum(const Vec &v)
    {
        Vec cumSum;
        cumSum[0] = v[0];

        for (uint i = 1; i < Dims; ++i) 
            cumSum[i] = cumSum[i - 1] + v[i];

        return cumSum;
    }

    template <typename Vec, typename Scalar = typename ScalarOf<Vec>::Type, uint Dims = DimsOf<Vec>::Value>
    HF_HDINLINE Vec exclusiveCumProd(const Vec &v)
    {
        Vec cumProd;
        cumProd[0] = 1;
        
        for (uint i = 1; i < Dims; ++i) 
            cumProd[i] = cumProd[i - 1] * v[i - 1];

        return cumProd;
    }

    template <typename Vec, typename Scalar = typename ScalarOf<Vec>::Type, uint Dims = DimsOf<Vec>::Value>
    HF_HDINLINE Vec inclusiveCumProd(const Vec &v)
    {
        Vec cumProd;
        cumProd[0] = v[0];

        for (uint i = 1; i < Dims; ++i) 
            cumProd[i] = cumProd[i - 1] * v[i];

        return cumProd;
    }

    HF_HDINLINE float saturate(float x)
    {
        return clamp(x, 0.0f, 1.0f);
    }

    template <uint Dims>
    HF_HDINLINE float saturate(const FloatN<Dims> &v)
    {
        return clamp(v, 0.0f, 1.0f);
    }

    /*
    template <typename T>
    HF_HINLINE T upperPowerOfTwo(const T& x)
    {
        T v = x;
        --v;
        for (size_t n = 1; n < 8 * sizeof(T); n <<= 1)
            v |= v >> n;
        ++v;
        return v;
    }
    */

    template <typename T>
    HF_HINLINE T upperPowerOfTwo(const T& x)
    {
        return T(pow(2.0f, ceil(log2(double(x)))));
    }

    template <typename T>
    HF_HINLINE T lowerPowerOfTwo(const T& x)
    {
        return T(pow(2.0f, floor(log2(double(x)))));
    }

    HF_HDINLINE Quaternion rotationFromTwoVectors(const Float3& u, const Float3& v)
    {
        const float m = sqrt(2.f + 2.f * dot(u, v));
        const Float3 w = (1.f / m) * cross(u, v);
        return Quaternion(0.5f * m, w.x, w.y, w.z);
    }

    HF_HDINLINE Quaternion extractRotation(const Float3x3& A, uint maxIter, float epsilon = 1e-9f)
    {
        Quaternion q(1.0f, Float3(0.0f));
    
        for (uint iter = 0; iter < maxIter; iter++)
        {
            const Float3x3 R(toMat4(q));

            const Float3 omega = (cross(R[0], A[0]) + cross(R[1], A[1]) + cross(R[2], A[2])) /
                                 (abs(dot(R[0], A[0]) + dot(R[1], A[1]) + dot(R[2], A[2])) + epsilon);

            const float w = length(omega);

            if (w < epsilon) break;

            q = angleAxis(w, (1.0f / w) * omega);
            q = normalize(q);
        }

        return q;
    }

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template<typename T>
    HF_HINLINE const T& formatArgument(const T& value)
    {
        return value;
    }

    HF_HINLINE const char* formatArgument(const std::string& value)
    {
        return value.c_str();
    }

    template<typename ... Args>
    HF_HINLINE std::string format(const std::string& format, const Args& ... args)
    {
        const size_t size = snprintf(nullptr, 0, format.c_str(), formatArgument(args)...) + 1; // Extra space for '\0'
        
        std::unique_ptr<char[]> buf(new char[size]);
        snprintf(buf.get(), size, format.c_str(), formatArgument(args)...);
        
        return std::string(buf.get(), buf.get() + size - 1);
    }

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF)

#endif /* HF_SIMULATOR_UTILITY_FUNCTIONS_HPP */
