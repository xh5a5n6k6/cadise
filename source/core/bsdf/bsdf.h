#pragma once

#include "core/bsdf/bsdfType.h"

#include "core/spectrum/spectrum.h"

namespace cadise {

class SurfaceIntersection;

class BSDF {
public:
    BSDF(const BSDFType& type);

    virtual Spectrum evaluate(const SurfaceIntersection& surfaceIntersection) const = 0;
    virtual Spectrum evaluateSample(SurfaceIntersection& surfaceIntersection) const = 0;

protected:
    BSDFType _type;
};

} // namespace cadise