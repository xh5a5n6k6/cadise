#pragma once

#include "core/light/category/areaLight.h"

#include "math/distribution/distribution2D.h"

#include <memory>

namespace cadise {

class Primitive;
template<typename T>
class Texture;

class EnvironmentLight : public AreaLight {
public:
    EnvironmentLight(const Primitive* const primitive,
                     const std::shared_ptr<Texture<Spectrum>>& environmentRadiance,
                     const Vector2S& resolution);

    Spectrum emittance(const Vector3R& emitDirection, const SurfaceIntersection& emitSurface) const override;
    Spectrum evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceInfo& surfaceInfo, real& t, real& pdf) const override;

    real evaluatePdfW(const SurfaceIntersection& surfaceIntersection, const real distance) const override;

private:
    const Primitive* _primitive;
    std::shared_ptr<Texture<Spectrum>> _environmentRadiance;
    Distribution2D _distribution;
};

} // namespace cadise