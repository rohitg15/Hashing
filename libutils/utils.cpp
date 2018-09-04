#include "utils.h"

uint64_t Utils::GetMsb(
    uint64_t x,
    int n,
    uint64_t w //               = 64
)
{
    if (n < 0)
    {
        return 0;
    }
    return (x >> (w - n - 1)) & 1;
}


uint64_t Utils::SetMsb(
    uint64_t x,
    int n,
    uint64_t v,
    uint64_t w                  //   = 64
)
{
    return (x | (v << (w - n - 1)));
}

bool Utils::TestBit(
    uint64_t x,
    uint64_t pos,
    uint64_t w
)
{
    return ((x >> pos) & 1);
}

uint64_t Utils::InvertXorRshift(
    uint64_t x,
    int r,
    int w                   // = 64
)
{
    uint64_t s = 0;
    for (int i = 0; i < 64; ++i)
    {
        s = SetMsb(s, i, GetMsb(x, i) ^ GetMsb(s, i - r));
    }
    return s;
}


uint32_t Utils::Ror(
    uint32_t x,
    int r,
    int w               //  = 32
)
{
    return (x << (w - r)) | (x >> r);
}

uint32_t Utils::Rol(
    uint32_t x,
    int r,
    int w               //  = 32
)
{
    return (x << r) | (x >> (w - r));
}

void Utils::CopyVec(
    const std::vector<uint8_t>& src,
    int low,
    int high,
    std::vector<uint8_t>& dest,
    int dlow
)
{
    if (dest.size() - dlow < high - low + 1)
    {
        throw std::runtime_error("Error in CopyVec:: Destination cannot hold source!");
    }
    for(int i = low, j = dlow; i <= high; ++i, ++j)
    {
        dest[j] = src[i];
    }
}