#pragma once

#include "core/light/light.h"

#include <memory>

namespace cadise {

class Primitive;

class AreaLight : public Light {
public:
    AreaLight(const Spectrum& albedo);

    Spectrum evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceGeometryInfo& surfaceGeometryInfo, real& t, real& pdf) const override;
    Spectrum color() const override;

    void setPrimitive(const std::shared_ptr<Primitive>& primitive);

private:
    std::weak_ptr<Primitive> _primitive;
    Spectrum _albedo;
};

} // namespace cadise