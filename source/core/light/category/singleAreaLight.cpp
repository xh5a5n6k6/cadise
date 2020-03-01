#include "core/light/category/singleAreaLight.h"

#include "core/integral-tool/sample/directLightSample.h"
#include "core/integral-tool/sample/positionSample.h"
#include "core/intersector/primitive/primitive.h"
#include "core/surfaceIntersection.h"
#include "core/texture/category/constantTexture.h"
#include "fundamental/assertion.h"
#include "math/constant.h"

#include <cmath>

namespace cadise {

SingleAreaLight::SingleAreaLight(const Primitive* const primitive,
                                 const Spectrum& color,
                                 const real watt,
                                 const bool isBackFaceEmit) :
    AreaLight(isBackFaceEmit),
    _primitive(primitive) {

    CADISE_ASSERT(primitive);
    CADISE_ASSERT_GT(primitive->area(), 0.0_r);

    const Spectrum unitWattColor  = color / color.sum();
    const Spectrum totalWattColor = unitWattColor * watt;

    _emitRadiance = std::make_shared<ConstantTexture<Spectrum>>(
                        totalWattColor / primitive->area() * constant::INV_PI);
}

Spectrum SingleAreaLight::emittance(const SurfaceIntersection& emitIntersection) const {
    const Vector3R& emitDirection = emitIntersection.wi();
    const Vector3R& emitNs        = emitIntersection.surfaceInfo().shadingNormal();
    if (!_canEmit(emitDirection, emitNs)) {

        return Spectrum(0.0_r);
    }
    
    // TODO: refactor here
    const Vector3R& uvw = emitIntersection.surfaceInfo().uvw();
    Spectrum sampleRadiance;
    _emitRadiance->evaluate(uvw, &sampleRadiance);

    return sampleRadiance;
}

void SingleAreaLight::evaluateDirectSample(DirectLightSample* const out_sample) const {
    CADISE_ASSERT(out_sample);

    PositionSample positionSample;
    _primitive->evaluatePositionSample(&positionSample);
    if (positionSample.pdfA() == 0.0_r) {

        return;
    }

    const Vector3R& emitPosition = positionSample.position();
    const Vector3R  emitVector   = out_sample->targetPosition() - emitPosition;
    const Vector3R& emitNs       = positionSample.shadingNormal();
    if (!_canEmit(emitVector, emitNs)) {

        return;
    }

    const real distance2              = emitVector.lengthSquared();
    const real emitDirectionDotEmitNs = (emitVector / std::sqrt(distance2)).absDot(emitNs);
    if (emitDirectionDotEmitNs == 0.0_r) {

        return;
    }

    // TODO: refactor here
    const Vector3R& uvw = positionSample.uvw();
    Spectrum sampleRadiance;
    _emitRadiance->evaluate(uvw, &sampleRadiance);

    out_sample->setEmitPosition(emitPosition);
    out_sample->setPdfW(positionSample.pdfA() * distance2 / emitDirectionDotEmitNs);
    out_sample->setRadiance(sampleRadiance);
}

real SingleAreaLight::evaluateDirectPdfW(
    const SurfaceIntersection& emitIntersection, 
    const Vector3R&            targetPosition) const {

    const Vector3R& emitPosition  = emitIntersection.surfaceInfo().point();
    const Vector3R& emitNs        = emitIntersection.surfaceInfo().shadingNormal();
    const Vector3R& emitDirection = emitIntersection.wi();
    const Vector3R  emitVector    = targetPosition - emitPosition;

    if (!_canEmit(emitDirection, emitNs)) {

        return 0.0_r;
    }

    const real emitDirectionDotEmitNs = emitDirection.absDot(emitNs);
    if (emitDirectionDotEmitNs == 0.0_r) {

        return 0.0_r;
    }

    const real pdfA = _primitive->evaluatePositionPdfA(emitPosition);
    const real pdfW = pdfA * emitVector.lengthSquared() / emitDirectionDotEmitNs;

    return pdfW;
}

real SingleAreaLight::approximatedFlux() const {
    PositionSample positionSample;
    _primitive->evaluatePositionSample(&positionSample);
    if (positionSample.pdfA() == 0.0_r) {

        return _defaultFlux();
    }

    const Vector3R& uvw = positionSample.uvw();
    Spectrum sampleRadiance;
    _emitRadiance->evaluate(uvw, &sampleRadiance);

    const real approximatedFlux = sampleRadiance.luminance() * _primitive->area() * constant::PI;
    if (approximatedFlux <= 0.0_r) {

        return _defaultFlux();
    }

    return approximatedFlux;
}

void SingleAreaLight::setEmitRadiance(const std::shared_ptr<Texture<Spectrum>>& emitRadiance) {
    CADISE_ASSERT(emitRadiance);

    _emitRadiance = emitRadiance;
}

} // namespace cadise