#include "core/light/category/pointLight.h"

#include "core/integral-tool/sample/directLightSample.h"
#include "core/surfaceInfo.h"
#include "fundamental/assertion.h"
#include "math/constant.h"

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
    const Vector3R emitVector   = out_sample->targetPosition() - emitPosition;
    
    CADISE_ASSERT(!emitVector.isZero());

    out_sample->setEmitPosition(emitPosition);
    out_sample->setRadiance(_intensity / emitVector.lengthSquared());
    out_sample->setPdfW(1.0_r);
}

real PointLight::evaluateDirectPdfW(
    const SurfaceIntersection& emitIntersection,
    const Vector3R&            targetPosition) const {

    return 0.0_r;
}

real PointLight::approximatedFlux() const {
    return constant::FOUR_PI * _intensity.luminance();
}

bool PointLight::isDeltaLight() const {
    return true;
}

} // namespace cadise