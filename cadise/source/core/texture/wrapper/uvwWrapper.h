#pragma once

#include "math/type/mathType.h"

namespace cadise {

class UvwWrapper {
public:
    virtual ~UvwWrapper();

    virtual void wrap(const Vector3R& uvw, Vector3R* const out_wrapUvw) const = 0;
};

} // namespace cadise