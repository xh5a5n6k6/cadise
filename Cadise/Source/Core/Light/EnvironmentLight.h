#pragma once

#include "Core/Light/AreaLight.h"

#include "Math/Distribution/Distribution2D.h"

#include <memory>

// forward declaration
namespace cadise
{
    class Primitive;

    template<typename T>
    class TTexture;
}

namespace cadise
{

class EnvironmentLight : public AreaLight
{
public:
    EnvironmentLight(
        const Primitive* const                     primitive,
        const std::shared_ptr<TTexture<Spectrum>>& environmentRadiance,
        const Vector2S&                            resolution);

    Spectrum emittance(const SurfaceIntersection& emitSi) const override;

    void evaluateDirectSample(DirectLightSample* const out_sample) const override;
    real evaluateDirectPdfW(
        const SurfaceIntersection& emitSi,
        const Vector3R&            targetPosition) const override;

    void evaluateEmitSample(EmitLightSample* const out_sample) const override;
    void evaluateEmitPdf(
        const Ray&      emitRay,
        const Vector3R& emitN,
        real* const     out_pdfA,
        real* const     out_pdfW) const override;

    real approximateFlux() const override;

    void setEmitRadiance(const std::shared_ptr<TTexture<Spectrum>>& emitRadiance) override;
    void setSceneBoundRadius(const real sceneBoundRadius) override;

private:
    void _updateApproximatedFlux();

    const Primitive* _primitive;

    std::shared_ptr<TTexture<Spectrum>> _environmentRadiance;
    Distribution2D                      _distribution;

    real _backgroundFlux;
    real _approximateFlux;
    real _sceneBoundRadius;
};

} // namespace cadise