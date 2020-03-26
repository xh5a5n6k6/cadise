#pragma once

#include "core/spectrum/spectrum.h"

namespace cadise {

class DirectLightSample;
class EmitLightSample;
class Ray;
class SurfaceInfo;
class SurfaceIntersection;

class Light {
public:
    virtual ~Light();

    virtual Spectrum emittance(const SurfaceIntersection& emitIntersection) const = 0;
    
    // light sampling used in next-event estimation (direct light)
    virtual void evaluateDirectSample(DirectLightSample* const out_sample) const = 0;
    virtual real evaluateDirectPdfW(
        const SurfaceIntersection& emitIntersection,
        const Vector3R&            targetPosition) const = 0;

    // light sampling used in light tracing methods
    // such as bidirectional path tracing and photon mapping
    virtual void evaluateEmitSample(EmitLightSample* const out_sample) const = 0;
    virtual void evaluateEmitPdf(
        const Ray&      emitRay,
        const Vector3R& emitN,
        real* const     out_pdfA,
        real* const     out_pdfW) const = 0;

    virtual real approximatedFlux() const = 0;

    virtual bool isDeltaLight() const = 0;

    // HACK
    // default this method does nothing,
    // only environment light would override this method
    virtual void setSceneBoundRadius(const real sceneBoundRadius);
};

} // namespace cadise