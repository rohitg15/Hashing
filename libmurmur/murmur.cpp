#include "murmur.h"

Murmur::Murmur(
    uint64_t m2Seed
) : m_m2Seed(m2Seed)
{}

uint64_t Murmur::Murmur2(
    const void *key,
    int len
) const
{
    const uint64_t m = ( static_cast<uint64_t>(0xc6a4a793) << 32) | 0x5bd1e995;
    const int r = 24;

    uint64_t h = m_m2Seed ^ len;
    const unsigned char *data = reinterpret_cast<const unsigned char*>(key);
    while (len >= 8)
    {
        uint64_t k = *(reinterpret_cast<const uint64_t*>(data));

        k *= m;
        k ^= k >> r;
        k *= m;
        
        h *= m;
        h ^= k;

        data += 8;
        len -= 8;
    }

    if (len > 0 && len < 8)
    {
        h ^= data[len - 1] <<  ( (len - 1) * 8 );
        h *= m;
    }

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}