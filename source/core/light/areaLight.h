#pragma once

#include "core/light/light.h"

#include "core/shape/shape.h"

#include "math/vector.h"

#include <memory>

namespace cadise {

class AreaLight : public Light {
public:
    AreaLight(std::shared_ptr<Shape> shape, Vector3 albedo);

    Vector3 evaluateSampleRadiance(Vector3 &lightDirection, SurfaceInfo &surfaceInfo, float &t, float &pdf) override;
    Vector3 position() override;
    Vector3 color() override;

    std::shared_ptr<Shape> shape();

private:
    std::shared_ptr<Shape> _shape;
    Vector3 _albedo;
};

} // namespace cadise