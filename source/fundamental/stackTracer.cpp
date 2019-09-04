#include "fundamental/stackTracer.h"

#include "fundamental/os.h"

#if defined(CADISE_OS_WINDOWS)
    #include "third-party/tp-StackWalker.h"
#elif defined(CADISE_OS_LINUX)
    #include <execinfo.h>
    #include <iostream>
#endif

namespace cadise {

StackTracer::StackTracer() = default;

void StackTracer::trace() const {

#if defined(CADISE_OS_WINDOWS)
    WindowsStackWalker stackWalker;
    stackWalker.ShowCallstack();

#elif defined(CADISE_OS_LINUX)
    void* traceBuffer[MAX_STACK_TRACE_DEPTH];
    int32 traceDepth = backtrace(traceBuffer, MAX_STACK_TRACE_DEPTH);
    char** traceMessages = backtrace_symbols(traceBuffer, traceDepth);

    for (int32 index = 0; index < traceDepth; index++) {
        std::cerr << traceMessages[index] << std::endl;
    }

#endif

}

} // namespace cadise