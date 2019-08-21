#pragma once

#include "core/spectrum/spectrum.h"

namespace cadise {

class SurfaceInfo;
class SurfaceIntersection;

class Light {
public:
    virtual Spectrum emittance(const Vector3R& emitDirection, const SurfaceInfo& emitSurface) const = 0;
    virtual Spectrum evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceInfo& surfaceInfo, real& t, real& pdf) const = 0;

    virtual real evaluatePdfW(const SurfaceIntersection& surfaceIntersection, const real distance) const = 0;

    virtual bool isDeltaLight() const = 0;
};

} // namespace cadise