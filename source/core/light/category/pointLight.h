#pragma once

#include "core/light/light.h"

namespace cadise {

class PointLight : public Light {
public:
    PointLight(const Vector3R& position, const Spectrum& color);

    Spectrum emittance(const Vector3R& emitDirection, const SurfaceInfo& emitSurface) const override;
    Spectrum evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceInfo& surfaceInfo, real& t, real& pdf) const override;

    real evaluatePdfW(const SurfaceIntersection& surfaceIntersection, const real distance) const override;

    bool isDeltaLight() const override;

private:
    Vector3R _position;
    Spectrum _color;
};

} // namespace cadise