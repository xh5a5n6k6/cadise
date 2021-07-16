#pragma once

#include "core/surface/bsdfComponents.h"
#include "core/surface/bsdfLobes.h"
#include "core/spectrum/spectrum.h"

namespace cadise {

class BsdfSample;
class SurfaceIntersection;
class TransportInfo;

class Bsdf {
public:
    Bsdf();
    explicit Bsdf(const BsdfLobes& lobes);
    Bsdf(const BsdfLobes& lobes, const BsdfComponents components);

    virtual ~Bsdf();

    virtual Spectrum evaluate(
        const TransportInfo&       info,
        const SurfaceIntersection& si) const = 0;

    virtual void evaluateSample(
        const TransportInfo&       info,
        const SurfaceIntersection& si,
        BsdfSample* const          out_sample) const = 0;

    virtual real evaluatePdfW(
        const TransportInfo&       info,
        const SurfaceIntersection& si) const = 0;

    virtual ELobe lobe(const BsdfComponents component) const = 0;

    const BsdfLobes& lobes() const;
    BsdfComponents components() const;

protected:
    BsdfLobes      _lobes;
    BsdfComponents _components;
};

} // namespace cadise