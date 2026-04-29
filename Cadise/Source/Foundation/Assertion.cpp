#include "Foundation/Assertion.h"

#ifdef CS_DEBUG
    #ifdef CS_ASSERT_STACK_TRACE
        #include "Foundation/StackTracer.h"
    #endif

    #ifdef CS_ASSERT_ABORT
        #include <cstdlib>
    #endif

    namespace cadise
    {

    void assertionFailed()
    {

    #ifdef CS_ASSERT_STACK_TRACE
        const StackTracer stackTracer;
        stackTracer.trace();
    #endif

    #ifdef CS_ASSERT_ABORT
        std::abort();
    #endif

    }

    } // namespace cadise
#endif