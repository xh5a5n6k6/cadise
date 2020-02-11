#pragma once

#include "core/light/category/areaLight.h"

#include <memory>

namespace cadise {

class Primitive;
template<typename T>
class Texture;

class SingleAreaLight : public AreaLight {
public:
    SingleAreaLight(const Primitive* const primitive,
                    const Spectrum& color, 
                    const real watt, 
                    const bool isBackFaceEmit);

    Spectrum emittance(const Vector3R& emitDirection, const SurfaceIntersection& emitSurface) const override;
    Spectrum evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceInfo& surfaceInfo, real& t, real& pdf) const override;

    real evaluatePdfW(const SurfaceIntersection& surfaceIntersection, const real distance) const override;

    void setEmitRadiance(const std::shared_ptr<Texture<Spectrum>>& emitRadiance);

private:
    const Primitive* _primitive;
    std::shared_ptr<Texture<Spectrum>> _emitRadiance;
};

} // namespace cadise