#pragma once

#include "core/light/light.h"

#include <memory>

namespace cadise {

class Primitive;

class AreaLight : public Light {
public:
    AreaLight(const Vector3R& albedo);

    Vector3R evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceGeometryInfo& surfaceGeometryInfo, real& t, real& pdf) const override;
    Vector3R position() const override;
    Vector3R color() const override;

    void setPrimitive(const std::shared_ptr<Primitive>& primitive);

private:
    std::weak_ptr<Primitive> _primitive;
    Vector3R _albedo;
};

} // namespace cadise