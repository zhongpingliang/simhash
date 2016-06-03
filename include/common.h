/*==============================================================================
*   Copyright (C) 2016 All rights reserved.
*
*  File Name    : common.h
*  Author       : Zhongping Liang
*  Date         : 2016-05-19
*  Version      : 1.0
*  Description  : This file provides some global defines.
==============================================================================*/

#ifndef SIMHASH_COMMON_H_
#define SIMHASH_COMMON_H_

namespace simhash
{
typedef uint64_t hash_t;            //hash type
typedef uint32_t uint_t;
typedef double   real_t;
const uint32_t HASH_WIDTH = 64U;    //The bit numbers of hash type
const hash_t HASH_1 = static_cast<hash_t>(1UL); // It's one!
} // namespace simhash

#endif  //SIMHASH_COMMON_H_
