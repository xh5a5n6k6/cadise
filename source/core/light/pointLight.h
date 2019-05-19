#pragma once

#include "core/light/light.h"

#include "math/algebra.h"

namespace cadise {

class PointLight : public Light {
public:
    PointLight(Vector3 position, Vector3 color);

    Vector3 evaluateSampleRadiance(Vector3 &lightDirection, SurfaceInfo &surfaceInfo, float &t) override;
    Vector3 position();
    Vector3 color();

private:
    Vector3 _position;
    Vector3 _color;
};

} // namespace cadise