#pragma once

#include "core/light/light.h"

#include <memory>

namespace cadise {

class Primitive;

class AreaLight : public Light {
public:
    AreaLight(const Spectrum& color, const real watt, const bool isBackFaceEmit);

    Spectrum emittance(const Vector3R& emitDirection, const SurfaceInfo& emitSurface) const override;
    Spectrum evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceInfo& surfaceInfo, real& t, real& pdf) const override;

    real evaluatePdfW(const SurfaceIntersection& surfaceIntersection, const real distance) const override;

    bool isDeltaLight() const override;

    void calculateEmitRadiance();

    void setPrimitive(const std::shared_ptr<Primitive>& primitive);
    void setIsBackFaceEmit(const bool isBackFaceEmit);

private:
    std::weak_ptr<Primitive> _primitive;
    Spectrum _emitRadiance;

    Spectrum _color;
    real _watt;

    bool _isBackFaceEmit;
};

} // namespace cadise