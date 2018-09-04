#include "murmurcollisions.h"
#include <stdlib.h>
#include "utils.h"
#include <algorithm>
#include <assert.h>
#include <stdexcept>
#include <string.h>

MurmurCollisions::MurmurCollisions(
    const Murmur& murmur
) : m_murmur(murmur)
{}

void MurmurCollisions::GetRandomBytes(
    std::vector<uint8_t>& buf
)
{
    for (auto i = 0; i < buf.size(); ++i)
    {
        buf[i] = rand() & 0xFF;
    }
}

void MurmurCollisions::GetMaskedBuffer(
    const std::vector<uint8_t>& buf0,
    const std::vector<uint8_t>& mask,
    std::vector<uint8_t>& buf1
)
{
    if ( buf0.size() != buf1.size() )
    {
        throw std::runtime_error("Error in GetMaskedBuffer:: Buffers are not of equal size");
    }


    for (size_t i = 0; i < buf0.size(); ++i)
    {
        buf1[i] = buf0[i] ^ mask[ i % mask.size()];
    }
}

void MurmurCollisions::Invert8Bytes(
    std::vector<uint8_t>& buf,
    uint64_t invMask,
    int r,
    int w                   // = 64
)
{
    if (buf.size() % 8 != 0)
    {
        throw std::runtime_error("Error in Invert8Bytes:: Expecting buffer to have a size that is a multiple of 8");
    }

    uint64_t *pBuf = reinterpret_cast<uint64_t*>(buf.data());

    for (int i = 0; i < buf.size() >> 3; ++i)
    {
        uint64_t val = *pBuf, inverse = 0;
        val = (val * invMask) & 0xFFFFFFFFFFFFFFFF;
        inverse = Utils::InvertXorRshift(val, r, w);
        inverse = (inverse * invMask) & 0xFFFFFFFFFFFFFFFF;

        // copy 64 bit integer into the buffer of bytes (take care of endian-ness)
        memcpy(buf.data() + i * (sizeof(inverse)), &inverse, sizeof(inverse));
        ++pBuf;
    }
}

void MurmurCollisions::InvertMurmur3Key32Bit(
    std::vector<uint8_t>& buf,
    uint32_t invMask0,
    uint32_t invMask1,
    int r,
    int w               //  = 32
)
{
    if (buf.size() % 4 != 0)
    {
        throw std::runtime_error("Error in InvertMurmur3Key32Bit:: buffer size is not a multiple of 4");
    }

    // read 4 bytes at a time
    uint32_t *pBuf = reinterpret_cast<uint32_t*>(buf.data());
    for (int i = 0; i < buf.size() >> 2; ++i)
    {
        uint32_t val = *pBuf, inverse = 0;
        val = (val * invMask0) & 0xFFFFFFFF;
        inverse = Utils::Ror(val, r, w);
        inverse = (inverse * invMask1) & 0xFFFFFFFF;

        memcpy(buf.data() + (i * (sizeof(inverse))), &inverse, sizeof(inverse));
        ++pBuf;
    }

}


void MurmurCollisions::GetAllCombinations(
    const std::vector<uint8_t>& buf0,
    const std::vector<uint8_t>& buf1,
    uint64_t stateSize,
    uint64_t numCollisions,
    MurmurCollisions::MultiCollisionSet& collisions
)
{
    if (buf0.size() != buf1.size())
    {
        throw std::runtime_error("Error in GetAllCombinations:: the two buffers have unequal lengths");
    }

    uint64_t numChunks = buf0.size() / stateSize;
    for (uint64_t i = 0; i < numCollisions; ++i)
    {
        int pos = 0;
        std::vector<uint8_t> buf(buf0.size());
        for (uint64_t j = 0; j < numChunks; ++j)
        {
            int low = j * stateSize;
            int high = (j + 1) * stateSize - 1;
            if (Utils::TestBit(i, j))
            {
                // pick buf1
                Utils::CopyVec(buf1, low, high, buf, pos);
                pos += stateSize;
            }
            else
            {
                // pick buf0
                Utils::CopyVec(buf0, low, high, buf, pos);
                pos += stateSize;
            }
        }
        collisions.emplace_back(buf);
    }
}

MurmurCollisions::MultiCollisionSet MurmurCollisions::GetMulticollisions(
    uint8_t count,
    size_t stateSize
)
{
    const uint64_t invMask = 0x5f7a0ea7e59b19bd;
    std::vector<uint8_t> mask = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80};
    uint64_t numCollisions = static_cast<uint64_t>(1 << count);
    MurmurCollisions::MultiCollisionSet multiCollisionSet;

    srand(0);
    std::vector<uint8_t> buf0(stateSize * count), buf1(stateSize * count);
    GetRandomBytes(buf0);

    // the second buffer differs only in the 16th byte
    GetMaskedBuffer(buf0, mask, buf1);

    // Invert every 8 bytes
    Invert8Bytes(buf0, invMask, 24, 64);
    Invert8Bytes(buf1, invMask, 24, 64);

    uint64_t hash0 = m_murmur.Murmur2(buf0.data(), buf0.size());
    uint64_t hash1 = m_murmur.Murmur2(buf1.data(), buf1.size());
    
    assert(hash0 == hash1);
    GetAllCombinations(buf0, buf1, stateSize, numCollisions, multiCollisionSet);
    return multiCollisionSet;
}

MurmurCollisions::MultiCollisionSet MurmurCollisions::GetMurmur3MultiCollisions32bit(
    uint8_t count,
    size_t stateSize
)
{

    const uint32_t invMask0 = 0x56ed309b;
    const uint32_t invMask1 = 0xdee13bb1;
    std::vector<uint8_t> mask = {0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x80};
    uint64_t numCollisions = static_cast<uint64_t>(1 << count);
    MurmurCollisions::MultiCollisionSet multiCollisionSet;

    // use crypto-random here for good randomness
    srand(0xFEEDBAAC);
    std::vector<uint8_t> buf0(stateSize * count), buf1(stateSize * count);
    GetRandomBytes(buf0);

    // the second buffer differs only in the 16th byte
    GetMaskedBuffer(buf0, mask, buf1);

    // Invert every 4 bytes
    InvertMurmur3Key32Bit(buf0, invMask0, invMask1, 15, 32 );
    InvertMurmur3Key32Bit(buf1, invMask0, invMask1, 15, 32 );

    GetAllCombinations(buf0, buf1, stateSize, numCollisions, multiCollisionSet);
    return multiCollisionSet;
}
