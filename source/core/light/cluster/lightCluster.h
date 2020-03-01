#pragma once

#include "math/type/fundamentalType.h"

namespace cadise {

class Light;

class LightCluster {
public:
    virtual ~LightCluster();

    virtual const Light* sampleOneLight(real* const out_pdf) const = 0;
};

} // namespace cadise