#pragma once

#include "core/light/light.h"

#include "math/vector.h"

namespace cadise {

class PointLight : public Light {
public:
    PointLight(Vector3F position, Vector3F color);

    Vector3F evaluateSampleRadiance(Vector3F &lightDirection, SurfaceInfo &surfaceInfo, float &t, float &pdf) override;
    Vector3F position();
    Vector3F color();

private:
    Vector3F _position;
    Vector3F _color;
};

} // namespace cadise