#pragma once

#include "core/light/light.h"

namespace cadise {

class AreaLight : public Light {
public:
    AreaLight();
    AreaLight(const bool isBackFaceEmit);

    Spectrum emittance(const SurfaceIntersection& emitIntersection) const override = 0;

    void evaluateDirectSample(DirectLightSample* const out_sample) const override = 0;
    real evaluateDirectPdfW(
        const SurfaceIntersection& emitIntersection,
        const Vector3R&            targetPosition) const override = 0;

    void evaluateEmitSample(EmitLightSample* const out_sample) const override = 0;
    void evaluateEmitPdf(
        const Ray&  emitRay,
        real* const out_pdfA,
        real* const out_pdfW) const override = 0;

    real approximatedFlux() const override = 0;

    bool isDeltaLight() const override;

    void setIsBackFaceEmit(const bool isBackFaceEmit);

protected:
    bool _canEmit(const Vector3R& direction, const Vector3R& N) const;
    real _defaultFlux() const;

    bool _isBackFaceEmit;
};

} // namespace cadise