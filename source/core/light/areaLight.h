#pragma once

#include "core/light/light.h"

#include "core/shape/shape.h"

#include "math/vector.h"

#include <memory>

namespace cadise {

class AreaLight : public Light {
public:
    AreaLight(std::shared_ptr<Shape> shape, Vector3F albedo);

    Vector3F evaluateSampleRadiance(Vector3F &lightDirection, SurfaceInfo &surfaceInfo, float &t, float &pdf) override;
    Vector3F position() override;
    Vector3F color() override;

    std::shared_ptr<Shape> shape();

private:
    std::shared_ptr<Shape> _shape;
    Vector3F _albedo;
};

} // namespace cadise