#include "fundamental/assertion.h"

#ifdef CS_DEBUG
    #ifdef CS_ASSERT_STACK_TRACE
        #include "fundamental/stackTracer.h"
    #endif

    #ifdef CS_ASSERT_ABORT
        #include <cstdlib>
    #endif

    namespace cadise
    {

    void assertionFailed()
    {

    #ifdef CS_ASSERT_STACK_TRACE
        StackTracer stackTracer;
        stackTracer.trace();
    #endif

    #ifdef CS_ASSERT_ABORT
        std::abort();
    #endif

    }

    } // namespace cadise
#endif