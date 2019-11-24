#pragma once

#include "core/light/light.h"

namespace cadise {

class AreaLight : public Light {
public:
    AreaLight(const bool isBackFaceEmit);

    Spectrum emittance(const Vector3R& emitDirection, const SurfaceIntersection& emitSurface) const override = 0;
    Spectrum evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceInfo& surfaceInfo, real& t, real& pdf) const override = 0;

    real evaluatePdfW(const SurfaceIntersection& surfaceIntersection, const real distance) const override = 0;

    bool isDeltaLight() const override;

    void setIsBackFaceEmit(const bool isBackFaceEmit);

protected:
    bool _isBackFaceEmit;
};

} // namespace cadise