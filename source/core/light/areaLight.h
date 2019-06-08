#pragma once

#include "core/light/light.h"

#include "core/shape/shape.h"

#include "math/vector.h"

#include <memory>

namespace cadise {

class AreaLight : public Light {
public:
    AreaLight(std::shared_ptr<Shape> shape, Vector3R albedo);

    Vector3R evaluateSampleRadiance(Vector3R &lightDirection, SurfaceInfo &surfaceInfo, real &t, real &pdf) override;
    Vector3R position() override;
    Vector3R color() override;

    std::shared_ptr<Shape> shape();

private:
    std::shared_ptr<Shape> _shape;
    Vector3R _albedo;
};

} // namespace cadise