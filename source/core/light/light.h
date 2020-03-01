#pragma once

#include "core/spectrum/spectrum.h"

namespace cadise {

class DirectLightSample;
class SurfaceInfo;
class SurfaceIntersection;

class Light {
public:
    virtual Spectrum emittance(const SurfaceIntersection& emitIntersection) const = 0;
    
    // light sampling used in next-event estimation (direct light)
    virtual void evaluateDirectSample(DirectLightSample* const out_sample) const = 0;
    virtual real evaluateDirectPdfW(
        const SurfaceIntersection& emitIntersection,
        const Vector3R&            targetPosition) const = 0;

    // light sampling used in light tracing methods
    // such as bidirectional path tracing and photon mapping
    // virtual Spectrum evaluateEmitSample() const = 0;
    // virtual real evaluateEmitPdfW() const = 0;

    virtual real approximatedFlux() const = 0;

    virtual bool isDeltaLight() const = 0;
};

} // namespace cadise