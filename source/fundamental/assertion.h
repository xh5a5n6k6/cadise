#pragma once

#include "fundamental/setting.h"

/*
    this is simple custom assert macro
    Reference Note: 
    http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/

    T.C. Chang also has a great implementation for stack trace
    when assertion fails.
    Reference Code: 
    https://github.com/TzuChieh/Photon-v2/blob/master/Engine/Source/Common/assertion.h
*/
#ifdef CADISE_DEBUG
    #include <iostream>
    #include <string>

    namespace cadise {
        void assertionFailed();
    } // namespace cadise

    #define CADISE_ASSERT_MSG(condition, message) \
        do { \
            if(!(condition)) { \
                std::cerr << "ASSERT FAIL. It occurs at " \
                          << __FILE__ << ", line: " << __LINE__ \
                          << ", condition: " << #condition; \
                \
                if(!std::string((message)).empty()) { \
                    std::cerr << ", message: " << ((message)); \
                } \
                \
                std::cerr << std::endl; \
                \
                cadise::assertionFailed(); \
            } \
        } while(0)
#else
    #define CADISE_ASSERT_MSG(condition, message)
#endif

#define CADISE_ASSERT(condition) \
    CADISE_ASSERT_MSG(condition, "")

// check if a is equal to b
#define CADISE_ASSERT_EQ(a, b) \
    CADISE_ASSERT(a == b)

// check if a is not equal to b
#define CADISE_ASSERT_NE(a, b) \
    CADISE_ASSERT(a != b)

// check if a is less than b
#define CADISE_ASSERT_LT(a, b) \
    CADISE_ASSERT(a < b)

// check if a is not greater than b
#define CADISE_ASSERT_LE(a, b) \
    CADISE_ASSERT(a <= b)

// check if a is greater than b
#define CADISE_ASSERT_GT(a, b) \
    CADISE_ASSERT(a > b)

// check if a is not less than b
#define CADISE_ASSERT_GE(a, b) \
    CADISE_ASSERT(a >= b)

// check if a is not less than b
// and not greater than c
#define CADISE_ASSERT_RANGE_INCLUSIVE(a, b, c) \
    CADISE_ASSERT_GE(a, b); \
    CADISE_ASSERT_LE(a, c)
