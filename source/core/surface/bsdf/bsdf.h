#pragma once

#include "core/surface/bsdfType.h"
#include "core/spectrum/spectrum.h"

namespace cadise {

class SurfaceIntersection;
class TransportInfo;

class Bsdf {
public:
    Bsdf();
    Bsdf(const BsdfType& type);

    virtual ~Bsdf();

    virtual Spectrum evaluate(
        const TransportInfo&       transportInfo,
        const SurfaceIntersection& surfaceIntersection) const = 0;

    virtual Spectrum evaluateSample(
        const TransportInfo& transportInfo,
        SurfaceIntersection& surfaceIntersection) const = 0;

    virtual real evaluatePdfW(
        const TransportInfo&       transportInfo,
        const SurfaceIntersection& surfaceIntersection) const = 0;

    const BsdfType& type() const;

protected:
    BsdfType _type;
};

} // namespace cadise