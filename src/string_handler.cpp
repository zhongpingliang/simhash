/*==============================================================================
*   Copyright (C) 2016 All rights reserved.
*
*  File Name    : string_handler.cpp
*  Author       : Zhongping Liang
*  Date         : 2016-04-25
*  Version      : 1.0
*  Description  : This file provides implement of the StringHandler.
==============================================================================*/

#include "string_handler.h"
#include <apsara/common/string_tools.h>

namespace simhash
{

const std::string StringHandler::WHITE_CHARS = " \t\n\r\f\v";

void StringHandler::SplitString(const std::string &str,
        std::vector<std::string> &strList, const std::string &delims)
{
    //Init.
    strList.clear();

    //Check params.
    if (str.empty())
    {
        return;
    }

    //Split.
    std::string::size_type size = str.size();
    std::string::size_type i = 0;
    std::string::size_type j = 0;
    const std::string &realDelims = delims.empty() ? WHITE_CHARS : delims;
    while (i <= size)
    {
        j = str.find_first_of(realDelims, i);
        if (std::string::npos == j)
        {
            j = size;
        }
        strList.push_back(str.substr(i, j - i));
        i = j + 1;
    }
}

void StringHandler::TrimString(const std::string &iStr, std::string &oStr,
            const std::string &trimChars)
{
    LeftTrimString(iStr, oStr, trimChars);
    RightTrimString(oStr, oStr, trimChars);
}

void StringHandler::LeftTrimString(const std::string &iStr, std::string &oStr,
            const std::string &trimChars)
{
    //Check params.
    if (iStr.empty())
    {
        oStr.clear();
        return;
    }
    const std::string &rTrimChars = trimChars.empty() ? WHITE_CHARS : trimChars;
    //Trim left.
    std::string::size_type i = iStr.find_first_not_of(rTrimChars);
    //write to result.
    if (std::string::npos == i)
    {
        oStr.clear();
        return;
    }
    if (&iStr == &oStr)
    {
        oStr.erase(0, i);
        return;
    }
    oStr.assign(iStr, i, std::string::npos);
}

void StringHandler::RightTrimString(const std::string &iStr, std::string &oStr,
            const std::string &trimChars)
{
    //Check params.
    if (iStr.empty())
    {
        oStr.clear();
        return;
    }
    const std::string &rTrimChars = trimChars.empty() ? WHITE_CHARS : trimChars;
    //Trim right.
    std::string::size_type i = iStr.find_last_not_of(rTrimChars);
    //write to result.
    if (std::string::npos == i)
    {
        oStr.clear();
        return;
    }
    if (&iStr == &oStr)
    {
        oStr.erase(i + 1);
        return;
    }
    oStr.assign(iStr, 0, i + 1);
}
} // namespace simhash
