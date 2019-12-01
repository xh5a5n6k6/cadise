#pragma once

#include "core/bsdf/bsdfType.h"
#include "core/spectrum/spectrum.h"

namespace cadise {

class SurfaceIntersection;

class Bsdf {
public:
    Bsdf();
    Bsdf(const BsdfType& type);

    virtual Spectrum evaluate(const SurfaceIntersection& surfaceIntersection) const = 0;
    virtual Spectrum evaluateSample(SurfaceIntersection& surfaceIntersection) const = 0;

    virtual real evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const = 0;

    const BsdfType& type() const;

protected:
    BsdfType _type;
};

} // namespace cadise