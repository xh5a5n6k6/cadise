#pragma once

#include "fundamental/config.h"

/*
    this is simple custom assert macro
    Reference Note: 
    http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/

    T.C. Chang also has a great implementation for stack trace
    when assertion fails.
    Reference Code: 
    https://github.com/TzuChieh/Photon-v2/blob/master/Engine/Source/Common/assertion.h
*/
#ifdef CS_DEBUG
    #include <iostream>
    #include <string>

    namespace cadise 
    {
        void assertionFailed();
    } // namespace cadise

    #define CS_ASSERT_MSG(condition, message)\
        do\
        {\
            if(!(condition))\
            {\
                std::cerr << "ASSERT FAIL. It occurs at "\
                          << __FILE__ << ", line: " << __LINE__\
                          << ", condition: " << #condition;\
                \
                if(!std::string((message)).empty())\
                {\
                    std::cerr << ", message: " << ((message));\
                }\
                \
                std::cerr << std::endl;\
                \
                cadise::assertionFailed();\
            }\
        } while(0)
#else
    #define CS_ASSERT_MSG(condition, message)
#endif

#define CS_ASSERT(condition)\
    CS_ASSERT_MSG(condition, "")

// check if a is equal to b
#define CS_ASSERT_EQ(a, b)\
    CS_ASSERT(a == b)

// check if a is not equal to b
#define CS_ASSERT_NE(a, b)\
    CS_ASSERT(a != b)

// check if a is less than b
#define CS_ASSERT_LT(a, b)\
    CS_ASSERT(a < b)

// check if a is not greater than b
#define CS_ASSERT_LE(a, b)\
    CS_ASSERT(a <= b)

// check if a is greater than b
#define CS_ASSERT_GT(a, b)\
    CS_ASSERT(a > b)

// check if a is not less than b
#define CS_ASSERT_GE(a, b)\
    CS_ASSERT(a >= b)

// check if a is not less than b
// and not greater than c
#define CS_ASSERT_RANGE_INCLUSIVE(a, b, c)\
    CS_ASSERT_GE(a, b);\
    CS_ASSERT_LE(a, c)
