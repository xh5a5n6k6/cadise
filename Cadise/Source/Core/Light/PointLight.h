#pragma once

#include "Core/Light/Light.h"

#include "Math/TVector3.h"

namespace cadise
{

class PointLight : public Light 
{
public:
    PointLight(const Vector3R& position, const Spectrum& intensity);

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

    bool isDeltaLight() const override;

private:
    Vector3R _position;
    Spectrum _intensity;
};

} // namespace cadise