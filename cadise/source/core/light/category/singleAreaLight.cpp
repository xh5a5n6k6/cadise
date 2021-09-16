#include "core/light/category/singleAreaLight.h"

#include "core/integral-tool/sample/directLightSample.h"
#include "core/integral-tool/sample/emitLightSample.h"
#include "core/integral-tool/sample/positionSample.h"
#include "core/intersector/primitive/primitive.h"
#include "core/ray.h"
#include "core/surfaceIntersection.h"
#include "core/texture/category/tConstantTexture.h"
#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/random.h"
#include "math/tLocalCoordinateSystem3.h"
#include "math/warp/hemisphere.h"

#include <cmath>

namespace cadise 
{

SingleAreaLight::SingleAreaLight(
    const Primitive* const primitive,
    const Spectrum&        color,
    const real             watt,
    const bool             isBackFaceEmit) :
    
    AreaLight(isBackFaceEmit),
    _primitive(primitive)
{
    CS_ASSERT(primitive);
    CS_ASSERT_GT(primitive->area(), 0.0_r);

    const Spectrum unitWattColor  = color.div(color.sum());
    const Spectrum totalWattColor = unitWattColor.mul(watt);

    _emitRadiance = std::make_shared<TConstantTexture<Spectrum>>(
        totalWattColor.div(primitive->area() * constant::pi<real>));
}

Spectrum SingleAreaLight::emittance(const SurfaceIntersection& emitIntersection) const
{
    const Vector3R& emitDirection = emitIntersection.wi();
    const Vector3R& emitNs        = emitIntersection.surfaceDetail().shadingNormal();
    if (!_canEmit(emitDirection, emitNs))
    {
        return Spectrum(0.0_r);
    }
    
    // TODO: refactor here
    const Vector3R& uvw = emitIntersection.surfaceDetail().uvw();
    Spectrum sampleRadiance;
    _emitRadiance->evaluate(uvw, &sampleRadiance);

    return sampleRadiance;
}

void SingleAreaLight::evaluateDirectSample(DirectLightSample* const out_sample) const
{
    CS_ASSERT(out_sample);

    PositionSample positionSample;
    _primitive->evaluatePositionSample(&positionSample);
    if (!positionSample.isValid()) 
    {
        return;
    }

    const Vector3R& emitPosition = positionSample.position();
    const Vector3R  emitVector   = out_sample->targetPosition().sub(emitPosition);
    const Vector3R& emitNs       = positionSample.shadingNormal();
    if (!_canEmit(emitVector, emitNs))
    {
        return;
    }

    const real distance2              = emitVector.lengthSquared();
    const real emitDirectionDotEmitNs = emitVector.div(std::sqrt(distance2)).absDot(emitNs);
    if (emitDirectionDotEmitNs <= 0.0_r)
    {
        return;
    }

    // TODO: refactor here
    const Vector3R& uvw = positionSample.uvw();
    Spectrum sampleRadiance;
    _emitRadiance->evaluate(uvw, &sampleRadiance);

    out_sample->setEmitPosition(emitPosition);
    out_sample->setEmitNormal(emitNs);
    out_sample->setPdfW(positionSample.pdfA() * distance2 / emitDirectionDotEmitNs);
    out_sample->setRadiance(sampleRadiance);
}

real SingleAreaLight::evaluateDirectPdfW(
    const SurfaceIntersection& emitIntersection, 
    const Vector3R&            targetPosition) const
{
    const Vector3R& emitPosition  = emitIntersection.surfaceDetail().position();
    const Vector3R& emitNs        = emitIntersection.surfaceDetail().shadingNormal();
    const Vector3R& emitDirection = emitIntersection.wi();
    const Vector3R  emitVector    = targetPosition.sub(emitPosition);

    if (!_canEmit(emitDirection, emitNs)) 
    {
        return 0.0_r;
    }

    const real emitDirectionDotEmitNs = emitDirection.absDot(emitNs);
    if (emitDirectionDotEmitNs <= 0.0_r)
    {
        return 0.0_r;
    }

    const real pdfA = _primitive->evaluatePositionPdfA(emitPosition);
    const real pdfW = pdfA * emitVector.lengthSquared() / emitDirectionDotEmitNs;

    return pdfW;
}

void SingleAreaLight::evaluateEmitSample(EmitLightSample* const out_sample) const 
{
    CS_ASSERT(out_sample);

    PositionSample positionSample;
    _primitive->evaluatePositionSample(&positionSample);
    if (!positionSample.isValid())
    {
        return;
    }

    const Vector3R& Ns = positionSample.shadingNormal();

    LCS3R lcs;
    lcs.initializeViaUnitY(Ns);

    const std::array<real, 2> sample = { Random::nextReal(), Random::nextReal() };
    Vector3R emitDirection;
    real pdfW;
    Hemisphere::cosineWeightedSampling(sample, &emitDirection, &pdfW);

    // transform emitDirection to world coordinate
    emitDirection = lcs.localToWorld(emitDirection);
    emitDirection.normalizeLocal();

    // if backFaceEmit, emitDirection needs to be reverse
    if (!_canEmit(emitDirection, Ns)) 
    {
        emitDirection.negateLocal();
    }

    Spectrum sampleRadiance;
    _emitRadiance->evaluate(positionSample.uvw(), &sampleRadiance);

    out_sample->setEmitPosition(positionSample.position());
    out_sample->setEmitDirection(emitDirection);
    out_sample->setEmitNormal(Ns);
    out_sample->setRadiance(sampleRadiance);
    out_sample->setPdfA(positionSample.pdfA());
    out_sample->setPdfW(pdfW);
}

void SingleAreaLight::evaluateEmitPdf(
    const Ray&      emitRay,
    const Vector3R& emitN,
    real* const     out_pdfA,
    real* const     out_pdfW) const 
{
    CS_ASSERT(out_pdfA);
    CS_ASSERT(out_pdfW);

    const real cosTheta = emitRay.direction().absDot(emitN);

    *out_pdfA = _primitive->evaluatePositionPdfA(emitRay.origin());
    *out_pdfW = cosTheta * constant::rcp_pi<real>;
}

real SingleAreaLight::approximateFlux() const 
{
    PositionSample positionSample;
    _primitive->evaluatePositionSample(&positionSample);
    if (positionSample.pdfA() <= 0.0_r)
    {
        return _defaultFlux();
    }

    const Vector3R& uvw = positionSample.uvw();
    Spectrum sampleRadiance;
    _emitRadiance->evaluate(uvw, &sampleRadiance);

    const real approximatedFlux = sampleRadiance.luminance() * _primitive->area() * constant::pi<real>;
    if (approximatedFlux <= 0.0_r) 
    {
        return _defaultFlux();
    }

    return approximatedFlux;
}

void SingleAreaLight::setEmitRadiance(const std::shared_ptr<TTexture<Spectrum>>& emitRadiance) 
{
    CS_ASSERT(emitRadiance);

    _emitRadiance = emitRadiance;
}

} // namespace cadise