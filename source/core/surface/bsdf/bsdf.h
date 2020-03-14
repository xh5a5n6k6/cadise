#pragma once

#include "core/surface/bsdfType.h"
#include "core/spectrum/spectrum.h"

namespace cadise {

class BsdfSample;
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

    virtual void evaluateSample(
        const TransportInfo&       transportInfo,
        const SurfaceIntersection& surfaceIntersection,
        BsdfSample* const          out_sample) const = 0;

    virtual real evaluatePdfW(
        const TransportInfo&       transportInfo,
        const SurfaceIntersection& surfaceIntersection) const = 0;

    const BsdfType& type() const;

protected:
    BsdfType _type;
};

} // namespace cadise