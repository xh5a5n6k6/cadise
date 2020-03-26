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

    Spectrum emittance(const SurfaceIntersection& emitIntersection) const override;

    void evaluateDirectSample(DirectLightSample* const out_sample) const override;
    real evaluateDirectPdfW(
        const SurfaceIntersection& emitIntersection, 
        const Vector3R&            targetPosition) const override;

    void evaluateEmitSample(EmitLightSample* const out_sample) const override;
    void evaluateEmitPdf(
        const Ray&      emitRay,
        const Vector3R& emitN,
        real* const     out_pdfA,
        real* const     out_pdfW) const override;

    real approximatedFlux() const override;
    
    void setSceneBoundRadius(const real sceneBoundRadius) override;

private:
    void _updateApproxmiatedFlux();

    const Primitive* _primitive;
    std::shared_ptr<Texture<Spectrum>> _environmentRadiance;
    Distribution2D _distribution;

    real _backgroundFlux;
    real _approximatedFlux;
    real _sceneBoundRadius;
};

} // namespace cadise