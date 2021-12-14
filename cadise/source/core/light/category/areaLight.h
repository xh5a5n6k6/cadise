#pragma once

#include "core/light/light.h"

#include "core/spectrum/spectrum.h"

#include <memory>

namespace cadise { template<typename T> class TTexture; }

namespace cadise 
{

class AreaLight : public Light 
{
public:
    AreaLight();
    explicit AreaLight(const bool isBackFaceEmit);

    Spectrum emittance(const SurfaceIntersection& emitSi) const override = 0;

    void evaluateDirectSample(DirectLightSample* const out_sample) const override = 0;
    real evaluateDirectPdfW(
        const SurfaceIntersection& emitSi,
        const Vector3R&            targetPosition) const override = 0;

    void evaluateEmitSample(EmitLightSample* const out_sample) const override = 0;
    void evaluateEmitPdf(
        const Ray&      emitRay,
        const Vector3R& emitN,
        real* const     out_pdfA,
        real* const     out_pdfW) const override = 0;

    real approximateFlux() const override = 0;

    bool isDeltaLight() const override;

    virtual void setEmitRadiance(const std::shared_ptr<TTexture<Spectrum>>& emitRadiance) = 0;
    void setIsBackFaceEmit(const bool isBackFaceEmit);

protected:
    bool _canEmit(const Vector3R& direction, const Vector3R& N) const;
    real _defaultFlux() const;

    bool _isBackFaceEmit;
};

} // namespace cadise