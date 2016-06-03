#ifndef SIMHASH_TEST_H_
#define SIMHASH_TEST_H_

#include <iostream>

namespace simhash
{

}

#define TEST_EQUAL(x,y) \
do \
{ \
    if(x != y) \
    { \
        std::cerr << "Expression:\"" << #x << "==" << #y \
            << "\": Not equal, where x = " << x <<", and y = " << y << "." \
            << std::endl; \
    } \
} while (false)

#define TEST_TRUE(x) \
do \
{ \
    if (!x) \
    { \
        std::cerr << "Expression:\"" << #x << "\". Not True" << endl; \
    } \
} while (false)

#define OUT_HEX(x) \
do \
{ \
    std::cout << #x << "\t= " << std::hex << x << std::endl; \
} while (false)

#define OUT_DEC(x) \
do \
{ \
    std::cout << #x << "\t= " << std::dec << x << std::endl; \
} while (false)

#define LOG_X(x) \
do \
{ \
    std::cout << x << std::endl; \
} while (false)


#endif
