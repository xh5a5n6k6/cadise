#include "core/light/category/pointLight.h"

#include "core/integral-tool/sample/directLightSample.h"
#include "core/surfaceDetail.h"
#include "fundamental/assertion.h"
#include "math/constant.h"

#include <cmath>

namespace cadise {

PointLight::PointLight(const Vector3R& position, const Spectrum& intensity) :
    _position(position), 
    _intensity(intensity) {
}

Spectrum PointLight::emittance(const SurfaceIntersection& emitIntersection) const {
    return Spectrum(0.0_r);
}

void PointLight::evaluateDirectSample(DirectLightSample* const out_sample) const {
    CADISE_ASSERT(out_sample);

    const Vector3R emitPosition = _position;
    const Vector3R emitVector   = out_sample->targetPosition().sub(emitPosition);
    
    CADISE_ASSERT(!emitVector.isZero());

    const real distance2 = emitVector.lengthSquared();

    out_sample->setEmitPosition(emitPosition);
    out_sample->setEmitNormal(emitVector.div(std::sqrt(distance2)));
    out_sample->setRadiance(_intensity.div(distance2));
    out_sample->setPdfW(1.0_r);
}

real PointLight::evaluateDirectPdfW(
    const SurfaceIntersection& emitIntersection,
    const Vector3R&            targetPosition) const {

    return 0.0_r;
}

void PointLight::evaluateEmitSample(EmitLightSample* const out_sample) const {
    CADISE_ASSERT(out_sample);

    // TODO: implement here
}

void PointLight::evaluateEmitPdf(
    const Ray&      emitRay,
    const Vector3R& emitN,
    real* const     out_pdfA,
    real* const     out_pdfW) const {

    CADISE_ASSERT(out_pdfA);
    CADISE_ASSERT(out_pdfW);

    *out_pdfA = 0.0_r;
    // TODO: sphere sample pdf

}

real PointLight::approximateFlux() const {
    return constant::four_pi<real> * _intensity.luminance();
}

bool PointLight::isDeltaLight() const {
    return true;
}

} // namespace cadise