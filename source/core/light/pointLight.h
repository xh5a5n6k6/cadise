#pragma once

#include "core/light/light.h"

#include "math/vector.h"

namespace cadise {

class PointLight : public Light {
public:
    PointLight(Vector3R position, Vector3R color);

    Vector3R evaluateSampleRadiance(Vector3R &lightDirection, SurfaceInfo &surfaceInfo, real &t, real &pdf) override;
    Vector3R position();
    Vector3R color();

private:
    Vector3R _position;
    Vector3R _color;
};

} // namespace cadise