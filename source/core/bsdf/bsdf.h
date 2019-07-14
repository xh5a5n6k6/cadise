#pragma once

#include "core/bsdf/bsdfType.h"

#include "math/vector.h"

namespace cadise {

class SurfaceIntersection;

class BSDF {
public:
    BSDF(const BSDFType& type);

    virtual Vector3R evaluate(const SurfaceIntersection& surfaceIntersection) const = 0;
    virtual Vector3R evaluateSample(SurfaceIntersection& surfaceIntersection) const = 0;

protected:
    BSDFType _type;
};

} // namespace cadise