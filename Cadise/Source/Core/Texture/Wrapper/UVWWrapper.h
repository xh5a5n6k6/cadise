#pragma once

#include "Math/Type/MathType.h"

namespace cadise
{

class UVWWrapper
{
public:
    virtual ~UVWWrapper();

    virtual void wrap(
        const Vector3R& uvw,
        Vector3R* const out_wrapUvw) const = 0;
};

} // namespace cadise