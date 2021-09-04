#pragma once

#include "math/type/fundamentalType.h"

namespace cadise {

class ClParser {
public:
    ClParser();

    int32 parse(int32 argc, char* argv[]) const;
};

} // namespace cadise