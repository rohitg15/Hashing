#ifndef _UTILS_H_
#define _UTILS_H_


#include <stdint.h>
#include <vector>

class Utils
{
public:
    static uint64_t GetMsb(
        uint64_t x,
        int n,
        uint64_t w = 64
    );


    static uint64_t SetMsb(
        uint64_t x,
        int n,
        uint64_t v,
        uint64_t w = 64
    );

    static bool TestBit(
        uint64_t x,
        uint64_t pos,
        uint64_t w = 64
    );

    // Invert x ^= x >> r;
    static uint64_t InvertXorRshift(
        uint64_t x,
        int r,
        int w = 64
    );

    static void CopyVec(
        const std::vector<uint8_t>& src,
        int low,
        int high,
        std::vector<uint8_t>& dest,
        int dlow
    );
};

#endif  //  _UTILS_H_