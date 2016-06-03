/*==============================================================================
*   Copyright (C) 2016 All rights reserved.
*
*  File Name    : string_handler.h
*  Author       : Zhongping Liang
*  Date         : 2016-04-25
*  Version      : 1.0
*  Description  : This file provides declaration of the StringHandler.
==============================================================================*/

#ifndef _STRING_HANDLER_H
#define _STRING_HANDLER_H

#include <string>
#include <vector>

namespace simhash
{

class StringHandler
{
public :
    //static const std::string WHITE_CHARS = " \t\n\r\f\v";
    static const std::string WHITE_CHARS;

    /*
    *   @brief      This func split a string into word list by delimiters.
    *           Note that :
    *               1. Empty strings are treated as a legal substr, and are
    *           included in the result vector strList;
    *               2. When delims is empty, str will be split by white chars,
    *           they are " \t\n\r\f\v";
    *               3. str is split by any char in the delims.
    *               4. str and delims can point to the same string.
    *   @author     Zhongping Liang
    *   @date       2016-04-25.
    *   @param      str: the string to be split.
    *   @param      strList: a vector to save the split result.
    *   @param      delims: delimiter chars with which the input str is split.
    *   @return     void.
    */
    static void SplitString(const std::string &str, std::vector<std::string>
            &strList, const std::string &delims = "");

    /*
    *   @brief      These three funcs trim the head and tail chars by trimChars.
    *           Note that :
    *               1. iStr and oStr can point to the same string, and oStr and
    *           trimChars can also point to the same string.
    *               2. When trimChars is empty, iStr will be trim by white
    *           chars, they are " \t\n\r\f\v";
    *               3. iStr is trimed by any char in the trimChars.
    *   @author     Zhongping Liang
    *   @date       2016-04-26
    *   @param      iStr: the input string to be trimed.
    *   @param      oStr: the output string to save trimed result.
    *   @param      trimChars: trim chars with which iStr is trimed.
    *   @return     void.
    */
    static void TrimString(const std::string &iStr, std::string &oStr,
            const std::string &trimChars = "");
    static void LeftTrimString(const std::string &iStr, std::string &oStr,
            const std::string &trimChars = "");
    static void RightTrimString(const std::string &iStr, std::string &oStr,
            const std::string &trimChars = "");
};

} // namespace model_serving

#endif //_STRING_HANDLER_H
