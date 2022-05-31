#ifndef HF_SIMULATOR_UTILITY_MORTON_HPP
#define HF_SIMULATOR_UTILITY_MORTON_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename T, uint N, uint U = 8 * sizeof(T)>
    struct Morton;

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename T>
    struct Morton<T, 1, 32>
    {
        static constexpr uint Bits = 32;

        static HF_HDINLINE T Encode(T x)
        {
            return x;
        }

        static HF_HDINLINE T Decode(T x)
        {
            return x;
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename T>
    struct Morton<T, 1, 64>
    {
        static constexpr uint Bits = 64;

        static HF_HDINLINE T Encode(T x)
        {
            return x;
        }

        static HF_HDINLINE T Decode(T x)
        {
            return x;
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename T>
    struct Morton<T, 2, 32>
    {
        static constexpr uint Bits = 32 / 2;

        static HF_HDINLINE T Encode(T x)
        {
            x &= 0xFFFF;
            x = (x | (x << 8)) & 0xFF00FF;
            x = (x | (x << 4)) & 0xF0F0F0F;
            x = (x | (x << 2)) & 0x33333333;
            x = (x | (x << 1)) & 0x55555555;
            return x;
        }

        static HF_HDINLINE T Decode(T x)
        {
            x &= 0x55555555;
            x = (x | (x >> 1)) & 0x33333333;
            x = (x | (x >> 2)) & 0xF0F0F0F;
            x = (x | (x >> 4)) & 0xFF00FF;
            x = (x | (x >> 8)) & 0xFFFF;
            return x;
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename T>
    struct Morton<T, 3, 32>
    {
        static constexpr uint Bits = 32 / 3;

        static HF_HDINLINE T Encode(T x)
        {
            x &= 0x3FF;
            x = (x | (x << 16)) & 0x30000FF;
            x = (x | (x << 8))  & 0x300F00F;
            x = (x | (x << 4))  & 0x30C30C3;
            x = (x | (x << 2))  & 0x9249249;
            return x;
        }

        static HF_HDINLINE T Decode(T x)
        {
            x &= 0x9249249;
            x = (x | (x >> 2)) & 0x30C30C3;
            x = (x | (x >> 4)) & 0x300F00F;
            x = (x | (x >> 8)) & 0x30000FF;
            x = (x | (x >> 16)) & 0x3FF;
            return x;
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename T>
    struct Morton<T, 2, 64>
    {
        static constexpr uint Bits = 64 / 2;

        static HF_HDINLINE T Encode(T x)
        {
            x &= 0xFFFFFFFF;
            x = (x | (x << 16)) & 0xFFFF0000FFFF;
            x = (x | (x << 8))  & 0xFF00FF00FF00FF;
            x = (x | (x << 4))  & 0xF0F0F0F0F0F0F0F;
            x = (x | (x << 2))  & 0x3333333333333333;
            x = (x | (x << 1))  & 0x5555555555555555;
            return x;
        }

        static HF_HDINLINE T Decode(T x)
        {
            x &= 0x5555555555555555;
            x = (x | (x >> 1)) & 0x3333333333333333;
            x = (x | (x >> 2)) & 0xF0F0F0F0F0F0F0F;
            x = (x | (x >> 4)) & 0xFF00FF00FF00FF;
            x = (x | (x >> 8)) & 0xFFFF0000FFFF;
            x = (x | (x >> 16)) & 0xFFFFFFFF;
            return x;
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename T>
    struct Morton<T, 3, 64>
    {
        static constexpr uint Bits = 64 / 3;

        static HF_HDINLINE T Encode(T x)
        {
            x &= 0x1FFFFF;
            x = (x | (x << 32)) & 0x1F00000000FFFF;
            x = (x | (x << 16)) & 0x1F0000FF0000FF;
            x = (x | (x << 8)) & 0x100F00F00F00F00F;
            x = (x | (x << 4)) & 0x10C30C30C30C30C3;
            x = (x | (x << 2)) & 0x1249249249249249;
            return x;
        }

        static HF_HDINLINE T Decode(T x)
        {
            x &= 0x1249249249249249;
            x = (x | (x >> 2)) & 0x10C30C30C30C30C3;
            x = (x | (x >> 4)) & 0x100F00F00F00F00F;
            x = (x | (x >> 8)) & 0x1F0000FF0000FF;
            x = (x | (x >> 16)) & 0x1F00000000FFFF;
            x = (x | (x >> 32)) & 0x1FFFFF;
            return x;
        }
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF)

#endif /* HF_SIMULATOR_UTILITY_MORTON_HPP */
