#ifndef _MURMUR_H_
#define _MURMUR_H_

#include <stdint.h>

class Murmur
{
public:

    Murmur(
        uint64_t m2Seed
    );

    uint64_t Murmur2(
        const void* key,
        int len
    ) const;
    static void Murmur3();

private:
    uint64_t m_m2Seed;
};

#endif      //  _MURMUR_H_