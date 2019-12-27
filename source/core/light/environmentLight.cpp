#include "core/light/environmentLight.h"

#include "core/intersector/primitive/primitive.h"
#include "core/surfaceIntersection.h"
#include "core/texture/texture.h"
#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/random.h"

#include <cmath>
#include <vector>

namespace cadise {

EnvironmentLight::EnvironmentLight(const Primitive* const primitive,
                                   const std::shared_ptr<Texture<Spectrum>>& environmentRadiance,
                                   const Vector2S& resolution) :
    AreaLight(false),
    _primitive(primitive),
    _environmentRadiance(environmentRadiance),
    _distribution() {

    CADISE_ASSERT(primitive);
    CADISE_ASSERT(environmentRadiance);

    std::vector<real> weightedSampleRadiances(resolution.x() * resolution.y());
    for (std::size_t iy = 0; iy < resolution.y(); ++iy) {
        const std::size_t rowIndex = iy * resolution.x();
        const real sampleV = (static_cast<real>(iy) + 0.5_r) / static_cast<real>(resolution.y());
        
        // image's y coordinate is from bottom to top, but
        // spherical theta is from top to bottom.
        const real sinTheta = std::sin((1.0_r - sampleV) * constant::PI);

        for (std::size_t ix = 0; ix < resolution.x(); ++ix) {
            const real sampleU = (static_cast<real>(ix) + 0.5_r) / static_cast<real>(resolution.x());

            Spectrum sampleRadiance;
            environmentRadiance->evaluate({sampleU, sampleV, 0.0_r}, &sampleRadiance);

            weightedSampleRadiances[rowIndex + ix] = sampleRadiance.luminance() * sinTheta;
        }
    }

    _distribution = Distribution2D(weightedSampleRadiances.data(), resolution);
}

Spectrum EnvironmentLight::emittance(const Vector3R& emitDirection, const SurfaceIntersection& emitSurface) const {
    const Vector3R uvw = emitSurface.surfaceInfo().uvw();
    Spectrum sampleRadiance;
    _environmentRadiance->evaluate(uvw, &sampleRadiance);

    return sampleRadiance;
}

Spectrum EnvironmentLight::evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceInfo& surfaceInfo, real& t, real& pdf) const {
    const Vector3R offsetOrigin = surfaceInfo.point() + constant::RAY_EPSILON * surfaceInfo.geometryNormal();

    real uvPdf;
    const Vector2R uvSample = _distribution.sampleContinuous({Random::nextReal(), Random::nextReal()}, 
                                                             &uvPdf);

    Vector3R samplePosition;
    _primitive->uvwToPosition({uvSample.x(), uvSample.y(), 0.0_r}, &samplePosition);

    const Vector3R direction = samplePosition - offsetOrigin;
    t = direction.length();
    lightDirection = direction.normalize();

    const real sinTheta = std::sin((1.0_r - uvSample.y()) * constant::PI);
    if (sinTheta == 0.0_r) {
        return Spectrum(0.0_r);
    }
    
    pdf = uvPdf / (2.0_r * constant::PI * constant::PI * sinTheta);

    Spectrum sampleRadiance;
    _environmentRadiance->evaluate({uvSample.x(), uvSample.y(), 0.0_r}, &sampleRadiance);

    return sampleRadiance;
}

real EnvironmentLight::evaluatePdfW(const SurfaceIntersection& surfaceIntersection, const real distance) const {
    const Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
    const real sinTheta = std::sin((1.0_r - uvw.y()) * constant::PI);
    if (sinTheta == 0.0_r) {
        return 0.0_r;
    }

    real uvPdf = _distribution.pdfContinuous({uvw.x(), uvw.y()});

    return uvPdf / (2.0_r * constant::PI * constant::PI * sinTheta);
}

} // namespace cadise