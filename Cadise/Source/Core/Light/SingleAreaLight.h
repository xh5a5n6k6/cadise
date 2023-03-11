#pragma once

#include "Core/Light/AreaLight.h"

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

class SingleAreaLight : public AreaLight 
{
public:
    SingleAreaLight(
        const Primitive* const primitive,
        const Spectrum&        color, 
        const real             watt, 
        const bool             isBackFaceEmit);

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

private:
    const Primitive* _primitive;

    std::shared_ptr<TTexture<Spectrum>> _emitRadiance;
};

} // namespace cadise