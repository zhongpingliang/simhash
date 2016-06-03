/*==============================================================================
*   Copyright (C) 2016 All rights reserved.
*
*  File Name    : simhash.h
*  Author       : Zhongping Liang
*  Date         : 2016-05-19
*  Version      : 1.0
*  Description  : This file provides declaration of the Simhash.
==============================================================================*/

#ifndef SIMHASH_SIMHASH_H_
#define SIMHASH_SIMHASH_H_

#include <string>
#include <vector>

#include "common.h"

namespace simhash
{
/*
* class Simhash.
* Simhash provides operations on simhash.
*/
class Simhash
{
//typedefs
public :
    /* Type of hash function.*/
    typedef hash_t (*HashFunc) (const std::string &);
    /* 
    *  Type of hash feature. In the pair:
    *      first  - is feature, should be a hash value.
    *      second - is weight,  should be a double type.
    */
    typedef std::pair<hash_t        , real_t> HashFeatureType;
    /* 
    *  Type of string feature. In the pair:
    *      first  - is feature, should be a string. When using this type of 
    *               feature, user should add a hash function to function Build.
    *      second - is weight,  should be a double type.
    */
    typedef std::pair<std::string   , real_t> StringFeatureType;
//public functions
public :
    /*
    *   @brief      This func judges whether two given simhash values are near
    *           duplicates.
    *   @author     Zhongping Liang
    *   @date       2016-05-19
    *   @param      lhs: the left  hand simhash value.
    *   @param      rhs: the right hand simhash value.
    *   @param      d  : the maximum Hamming distance can be tolerated.
    *   @return     true, if the Hamming distance of lhs and rhs is not greater
    *           than d; false otherwise.
    */
    static bool IsNearDups(hash_t lhs, hash_t rhs, uint_t d = 3);
    /*
    *   @brief      This func calculates the Hamming distance between two given
    *           simhash values.
    *   @author     Zhongping Liang
    *   @date       2016-05-19
    *   @param      lhs: the left  hand simhash value.
    *   @param      rhs: the right hand simhash value.
    *   @return     the Hamming distance.
    */
    static uint_t GetHammingDistance(hash_t lhs, hash_t rhs);
    /*
    *   @brief      This func builds simhash value from HashFeatureType
    *           features.
    *   @author     Zhongping Liang
    *   @date       2016-05-19
    *   @param      features: the input features, each should be
    *           <hash_t, real_t> pair.
    *   @return     the simhash value of features.
    */
    static hash_t Build(const std::vector<HashFeatureType> &features);
    /*
    *   @brief      This func builds simhash value from StringFeatureType
    *           features.
    *   @author     Zhongping Liang
    *   @date       2016-05-19
    *   @param      features: the input features, each should be 
    *           <string, real_t> pair.
    *   @param      hasher: a hash function hashes the string feature to its
    *           hash fingerprint.
    *   @return     the simhash value of features.
    *   @desc       When using this overloaded build, you should given a hash
    *           function in parameter. The given hash function should take a
    *           string as input, and give out a hash_t as output meaning the
    *           hash value of the input string. This function simple calls 
    *           hasher for each feature. Note that hasher should not be null, or
    *           this func will do nothing. You can use JenkinsHash in hash.h
    *           conveniently.
    */
    static hash_t Build(const std::vector<StringFeatureType> &features,
        HashFunc hasher);
    /*
    *   @brief      This func convert simhash value into a binary string.
    *   @author     Zhongping Liang
    *   @date       2016-05-19
    *   @param      hash: the input simhash value.
    *   @param      ans : the output binary string.
    *   @return     void.
    */
    static void HashToBinaryString(hash_t hash, std::string& ans);
    /*
    *   @brief      This func convert binary string into simhash value.
    *   @author     Zhongping Liang
    *   @date       2016-05-19
    *   @param      str: the input binary string.
    *   @return     the output simhash value.
    */
    static hash_t BinaryStringToHash(const std::string& str);
//private functions
private :
    /* Build from holds, the length of holds should equal to HASH_WIDTH. */
    static hash_t Build(const std::vector<real_t> &holds);
    /* Flush holds by hash feature and its weight. */
    static void FlushHolds(hash_t hash, real_t weight,
        std::vector<real_t> &holds);
};
} // namespace simhash

#endif // SIMHASH_SIMHASH_H_
