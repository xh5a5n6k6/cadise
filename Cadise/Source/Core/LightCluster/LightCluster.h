#pragma once

#include "Math/Type/PrimitiveType.h"

// forward declaration
namespace cadise
{
    class Light;
}

namespace cadise
{

class LightCluster
{
public:
    virtual ~LightCluster();

    virtual const Light* sampleOneLight(real* const out_pdf) const = 0;
    virtual real evaluatePickLightPdf(const Light* const light) const = 0;
};

} // namespace cadise