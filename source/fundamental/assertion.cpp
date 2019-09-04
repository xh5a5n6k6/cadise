#include "fundamental/assertion.h"

#ifdef CADISE_DEBUG

    #ifdef CADISE_ASSERT_STACK_TRACE
        #include "fundamental/stackTracer.h"
    #endif

    #ifdef CADISE_ASSERT_ABORT
        #include <cstdlib>
    #endif

    namespace cadise {

    void assertionFailed() {

    #ifdef CADISE_ASSERT_STACK_TRACE
        StackTracer stackTracer;
        stackTracer.trace();
    #endif

    #ifdef CADISE_ASSERT_ABORT
        std::abort();
    #endif

    }

    } // namespace cadise

#endif