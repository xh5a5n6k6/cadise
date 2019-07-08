#pragma once

#include "core/light/light.h"

namespace cadise {

class PointLight : public Light {
public:
    PointLight(const Vector3R& position, const Vector3R& color);

    Vector3R evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceGeometryInfo& surfaceGeometryInfo, real& t, real& pdf) const override;
    Vector3R position() const override;
    Vector3R color() const override;

private:
    Vector3R _position;
    Vector3R _color;
};

} // namespace cadise