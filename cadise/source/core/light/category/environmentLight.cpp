#include "core/light/category/environmentLight.h"

#include "core/integral-tool/sample/directLightSample.h"
#include "core/intersector/primitive/primitive.h"
#include "core/surfaceIntersection.h"
#include "core/texture/tTexture.h"
#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/random.h"
#include "math/tVector2.h"

#include <cmath>
#include <vector>

namespace cadise 
{

EnvironmentLight::EnvironmentLight(
    const Primitive* const                     primitive,
    const std::shared_ptr<TTexture<Spectrum>>& environmentRadiance,
    const Vector2S&                            resolution) :
    
    AreaLight(false),
    _primitive(primitive),
    _environmentRadiance(environmentRadiance),
    _distribution(),
    _backgroundFlux(0.0_r),
    _approximateFlux(0.0_r),
    _sceneBoundRadius(0.0_r)
{
    CADISE_ASSERT(primitive);
    CADISE_ASSERT(environmentRadiance);

    std::vector<real> weightedSampleRadiances(resolution.x() * resolution.y());
    for (std::size_t iy = 0; iy < resolution.y(); ++iy) 
    {
        const std::size_t rowIndex = iy * resolution.x();
        const real        sampleV  = (static_cast<real>(iy) + 0.5_r) / static_cast<real>(resolution.y());
        
        // image's y coordinate is from bottom to top, but
        // spherical theta is from top to bottom.
        const real sinTheta = std::sin((1.0_r - sampleV) * constant::pi<real>);

        for (std::size_t ix = 0; ix < resolution.x(); ++ix) 
        {
            const real sampleU = (static_cast<real>(ix) + 0.5_r) / static_cast<real>(resolution.x());

            Spectrum sampleRadiance;
            environmentRadiance->evaluate({ sampleU, sampleV, 0.0_r }, &sampleRadiance);

            weightedSampleRadiances[rowIndex + ix] = sampleRadiance.luminance() * sinTheta;

            _backgroundFlux += weightedSampleRadiances[rowIndex + ix];
        }
    }

    _distribution = Distribution2D(weightedSampleRadiances.data(), resolution);

    // set dafulat scene radius
    this->setSceneBoundRadius(5000.0_r);
}

Spectrum EnvironmentLight::emittance(const SurfaceIntersection& emitIntersection) const 
{
    const Vector3R& uvw = emitIntersection.surfaceDetail().uvw();
    Spectrum sampleRadiance;
    _environmentRadiance->evaluate(uvw, &sampleRadiance);

    return sampleRadiance;
}

void EnvironmentLight::evaluateDirectSample(DirectLightSample* const out_sample) const 
{
    CADISE_ASSERT(out_sample);

    const std::array<real, 2> sample = { Random::nextReal(), Random::nextReal() };
    real uvPdf;
    const Vector2R uvSample = _distribution.sampleContinuous(sample, &uvPdf);

    Vector3R samplePosition;
    _primitive->uvwToPosition({ uvSample.x(), uvSample.y(), 0.0_r }, &samplePosition);

    const real sinTheta = std::sin((1.0_r - uvSample.y()) * constant::pi<real>);
    if (sinTheta <= 0.0_r)
    {
        return;
    }

    Spectrum sampleRadiance;
    _environmentRadiance->evaluate({ uvSample.x(), uvSample.y(), 0.0_r }, &sampleRadiance);

    out_sample->setEmitPosition(samplePosition);
    out_sample->setPdfW(uvPdf / (2.0_r * constant::pi<real> * constant::pi<real> * sinTheta));
    out_sample->setRadiance(sampleRadiance);
}

real EnvironmentLight::evaluateDirectPdfW(
    const SurfaceIntersection& emitIntersection, 
    const Vector3R&            targetPosition) const 
{
    const Vector3R& uvw      = emitIntersection.surfaceDetail().uvw();
    const real      sinTheta = std::sin((1.0_r - uvw.y()) * constant::pi<real>);
    if (sinTheta <= 0.0_r)
    {
        return 0.0_r;
    }

    const real uvPdf = _distribution.pdfContinuous({ uvw.x(), uvw.y() });

    return uvPdf / (2.0_r * constant::pi<real> * constant::pi<real> * sinTheta);
}

void EnvironmentLight::evaluateEmitSample(EmitLightSample* const out_sample) const
{
    CADISE_ASSERT(out_sample);

    // TODO: implement here
}

void EnvironmentLight::evaluateEmitPdf(
    const Ray&      emitRay,
    const Vector3R& emitN,
    real* const     out_pdfA,
    real* const     out_pdfW) const 
{
    CADISE_ASSERT(out_pdfA);
    CADISE_ASSERT(out_pdfW);

    // TODO: implement here
}

real EnvironmentLight::approximateFlux() const 
{
    return _approximateFlux;
}

void EnvironmentLight::setSceneBoundRadius(const real sceneBoundRadius) 
{
    CADISE_ASSERT_GE(sceneBoundRadius, 0.0_r);

    _sceneBoundRadius = sceneBoundRadius;

    _updateApproxmiatedFlux();
}

void EnvironmentLight::_updateApproxmiatedFlux() 
{
    _approximateFlux = 
        _backgroundFlux * constant::four_pi<real> * _sceneBoundRadius * _sceneBoundRadius;
}

} // namespace cadise