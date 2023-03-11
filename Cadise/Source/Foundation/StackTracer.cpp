#include "Foundation/StackTracer.h"

#include "Foundation/OS.h"

#if defined(CADISE_OS_WINDOWS)
    #include "ThirdParty/TP_StackWalker.h"
#elif defined(CADISE_OS_LINUX)
    #include <execinfo.h>
    #include <iostream>
#endif

namespace cadise 
{

StackTracer::StackTracer() = default;

void StackTracer::trace() const 
{

#if defined(CADISE_OS_WINDOWS)
    WindowsStackWalker stackWalker;
    stackWalker.ShowCallstack();

#elif defined(CADISE_OS_LINUX)
    void*  traceBuffer[MAX_STACK_TRACE_DEPTH];
    int32  traceDepth    = backtrace(traceBuffer, MAX_STACK_TRACE_DEPTH);
    char** traceMessages = backtrace_symbols(traceBuffer, traceDepth);

    for (int32 i = 0; i < traceDepth; ++i) 
    {
        std::cerr << traceMessages[i] << std::endl;
    }

#endif

}

} // namespace cadise