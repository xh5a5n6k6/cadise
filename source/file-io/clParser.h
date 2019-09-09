#pragma once

#include "math/fundamentalType.h"

namespace cadise {

class ClParser {
public:
    ClParser();

    void parse(int32 argc, const char* argv[]) const;

private:
};

} // namespace cadise