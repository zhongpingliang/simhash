/*==============================================================================
*   Copyright (C) 2016 All rights reserved.
*
*  File Name    : hash.h
*  Author       : Zhongping Liang
*  Date         : 2016-05-19
*  Version      : 1.0
*  Description  : This file provides declaration of the JenkinsHash.
==============================================================================*/

#ifndef SIMHASH_HASH_JENKINS_H
#define SIMHASH_HASH_JENKINS_H

#include <string>
#include "common.h"

namespace simhash 
{
    /*
    *   @brief      This func hashes the input string to its fingerprint.
    *   @author     Zhongping Liang
    *   @date       2016-05-19
    *   @param      str: the input string.
    *   @return     The fingerprint of the input string.
    *   @desc       The code was taken from the website :
    *           http://www.burtleburtle.net/bob/c/lookup3.c,
    *           and under a public domain licence on May 25, 2012.
    */
    hash_t JenkinsHash(const std::string &str);
}

#endif // SIMHASH_HASH_JENKINS_H
