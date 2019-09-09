#pragma once

#include "math/fundamentalType.h"

namespace cadise {

class StackTracer {
public:
    StackTracer();

    void trace() const;

private:
    static const int32 MAX_STACK_TRACE_DEPTH = 32;
};

} // namespace cadise