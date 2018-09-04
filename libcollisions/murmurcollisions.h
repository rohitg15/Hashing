#ifndef _MURMUR_COLLISIONS_H_
#define _MURMUR_COLLISIONS_H_

#include "murmur.h"
#include <cstdlib>
#include <vector>

class MurmurCollisions
{
public:
    MurmurCollisions(
        const Murmur& murmur
    );

    typedef std::vector< std::vector<uint8_t> > MultiCollisionSet;

    MultiCollisionSet GetMulticollisions(
        uint8_t count,
        size_t stateSize
    );

    MultiCollisionSet GetMurmur3MultiCollisions32bit(
        uint8_t count,
        size_t stateSize
    );

private:
    void GetRandomBytes(
        std::vector<uint8_t>& buf
    );

    void GetMaskedBuffer(
        const std::vector<uint8_t>& buf0,
        const std::vector<uint8_t>& mask,
        std::vector<uint8_t>& buf1
    );

    void Invert8Bytes(
        std::vector<uint8_t>& buf,
        uint64_t invMask,
        int r,
        int w = 64
    );

    void InvertMurmur3Key32Bit(
        std::vector<uint8_t>& buf,
        uint32_t invMask0,
        uint32_t invMask1,
        int r,
        int w = 32
    );

    void GetAllCombinations(
        const std::vector<uint8_t>& buf0,
        const std::vector<uint8_t>& buf1,
        uint64_t stateSize,
        uint64_t numCollisions,
        MultiCollisionSet& collisions
    );

    const Murmur& m_murmur;
};

#endif  //  _MURMUR_COLLISIONS_H_