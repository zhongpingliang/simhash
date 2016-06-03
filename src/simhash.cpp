#include "simhash.h"

#include <string>

namespace simhash
{

bool Simhash::IsNearDups(hash_t lhs, hash_t rhs, uint_t d)
{
    for (lhs ^= rhs; lhs && d; --d)
    {
        lhs &= lhs - 1;
    }
    if (!d && lhs)  //d == 0 and lhs != 0
    {
        return false;
    }
    return true;
}

uint_t Simhash::GetHammingDistance(hash_t lhs, hash_t rhs)
{
    uint_t ans = 0U;
    for (lhs ^= rhs; lhs; ++ans)
    {
        lhs &= lhs - 1;
    }
    return ans;
}

hash_t Simhash::Build(const std::vector<HashFeatureType> &features)
{
    std::vector<real_t> holds(HASH_WIDTH, 0.0);
    for (std::vector<HashFeatureType>::const_iterator iter = features.begin();
        features.end() != iter; ++iter)
    {
        FlushHolds(iter->first, iter->second, holds);
    }
    return Build(holds);
}

hash_t Simhash::Build(const std::vector<StringFeatureType> &features,
    HashFunc hasher)
{
    if (!hasher)    //Don't put a nullptr, or will do nothing.
    {
        return 0UL;
    }
    std::vector<real_t> holds(HASH_WIDTH, 0.0);
    for (std::vector<StringFeatureType>::const_iterator iter = features.begin();
        features.end() != iter; ++iter)
    {
        FlushHolds(hasher(iter->first), iter->second, holds);
    }
    return Build(holds);
}

hash_t Simhash::Build(const std::vector<real_t> &holds)
{
    if (HASH_WIDTH != holds.size()) //In no case, this will happen.
    {
        return 0UL;
    }
    hash_t ret = 0UL;
    for (uint_t i = 0; i < HASH_WIDTH; ++i)
    {
        if (holds[i] > 0.0)         //If holds at i greater than 0, sets bit
        {
            ret |= (HASH_1 << i);
        }
    }
    return ret;
}

void Simhash::FlushHolds(hash_t hash, real_t weight, std::vector<real_t> &holds)
{
    for (uint_t i = 0U; i < HASH_WIDTH; ++i)
    {
        if (((HASH_1 << i) & hash)) //If bit at j is 1, ++
        {
            holds[i] += weight;
        }
        else
        {
            holds[i] -= weight;     //If bit at j is 0, --
        }
    }
}

void Simhash::HashToBinaryString(hash_t hash, std::string& ans)
{
    ans.resize(HASH_WIDTH);
    uint_t i = HASH_WIDTH;
    do
    {
        ans.at(--i) = hash & HASH_1 ? '1' : '0';
        hash >>= HASH_1;
    } while (i);
}

hash_t Simhash::BinaryStringToHash(const std::string& str)
{
    hash_t ans = 0UL;
    for (std::string::const_iterator it = str.begin(); str.end() != it; ++it)
    {
        ans <<= HASH_1;
        if ('1' == *it)
        {
            ans |= HASH_1;
        }
    }
    return ans;
}
}
