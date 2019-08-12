#pragma once

#include "core/light/light.h"

namespace cadise {

class PointLight : public Light {
public:
    PointLight(const Vector3R& position, const Spectrum& color);

    Spectrum evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceInfo& surfaceInfo, real& t, real& pdf) const override;
    Spectrum color() const override;

private:
    Vector3R _position;
    Spectrum _color;
};

} // namespace cadise