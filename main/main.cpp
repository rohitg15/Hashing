#include "murmur.h"
#include "murmurcollisions.h"
#include <iostream>
#include <vector>


int main(int argc, char **argv)
{
    uint64_t seed = static_cast<uint64_t>(0xDEADBEEF);
    Murmur murmur(seed);
    MurmurCollisions collisions(murmur);
    MurmurCollisions::MultiCollisionSet collisionSet = collisions.GetMulticollisions(5, 16);

    std::cout << " Number of Collisions found # : " << collisionSet.size() << std::endl;
    for (const auto& collisionObj : collisionSet)
    {
        printf("%lu -> ", collisionObj.size());
        for(const auto& byte : collisionObj)
        {
            printf("%02x",byte);
        }
        if (collisionObj.size() > 0)
        {
            printf(" , %16llx\n", murmur.Murmur2(collisionObj.data(), collisionObj.size()));
        }   
    }

    return 0;
}