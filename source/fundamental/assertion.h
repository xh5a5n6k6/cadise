#pragma once

#include "fundamental/setting.h"

// this is simple custom assert macro
// Reference Note: http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/
//
// T.C. Chang also has a great implementation for stack trace
// when assertion fails.
// Reference Code: https://github.com/TzuChieh/Photon-v2/blob/master/Engine/Source/Common/assertion.h
//
#ifdef CADISE_DEBUG
    #include <iostream>

    namespace cadise {
        void assertionFailed();
    } // namespace cadise

    #define CADISE_ASSERT_MSG(condition, message) \
        do { \
            if(!(condition)) { \
                std::cerr << "ASSERT FAIL. It occurs at " \
                          << __FILE__ << ", line: " << __LINE__ \
                          << ", condition: " << #condition \
                          << std::endl; \
                cadise::assertionFailed(); \
            } \
        } while(0)
#else
    #define CADISE_ASSERT_MSG(condition, message)
#endif

#define CADISE_ASSERT(condition) \
    CADISE_ASSERT_MSG(condition, "")
