#pragma once

#include "core/light/light.h"

#include "core/shape/shape.h"

#include "math/vector.h"

#include <memory>

namespace cadise {

class AreaLight : public Light {
public:
    AreaLight(const std::shared_ptr<Shape> shape, const Vector3R albedo);

    Vector3R evaluateSampleRadiance(Vector3R &lightDirection, SurfaceInfo &surfaceInfo, real &t, real &pdf) const override;
    Vector3R position() const override;
    Vector3R color() const override;

    std::shared_ptr<Shape> shape() const;

private:
    std::shared_ptr<Shape> _shape;
    Vector3R _albedo;
};

} // namespace cadise