#include "core/bsdf/category/lambertianDiffuse.h"

#include "core/integral-tool/hemisphere.h"
#include "core/surfaceIntersection.h"
#include "core/texture/texture.h"
#include "core/texture/category/constantTexture.h"

#include "math/constant.h"
#include "math/math.h"
#include "math/random.h"

namespace cadise {

// Hack
LambertianDiffuse::LambertianDiffuse() :
    LambertianDiffuse(std::make_shared<ConstantTexture<Spectrum>>(0.5_r)) {
}

LambertianDiffuse::LambertianDiffuse(const std::shared_ptr<Texture<Spectrum>>& albedo) :
    Bsdf(BsdfType(BxdfType::DIFFUSE_REFLECTION)),
    _albedo(albedo) {
}

Spectrum LambertianDiffuse::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleSpectrum;
    _albedo->evaluate(uvw, &sampleSpectrum);

    return sampleSpectrum * constant::INV_PI;
}

Spectrum LambertianDiffuse::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    const Vector3R normal = surfaceIntersection.surfaceInfo().shadingNormal();

    Vector3R xAxis;
    Vector3R yAxis;
    Vector3R zAxis(normal);
    math::buildCoordinateSystem(zAxis, xAxis, yAxis);

    Vector2R uniformSample = Vector2R(random::nextReal(), random::nextReal());
    Vector3R sampleDir = hemisphere::cosineWeightedSampling(uniformSample);
    Vector3R outDirection = xAxis * sampleDir.x() + yAxis * sampleDir.y() + zAxis * sampleDir.z();
    outDirection = outDirection.normalize();

    real pdf = outDirection.absDot(zAxis) * constant::INV_PI;

    surfaceIntersection.setWo(outDirection);
    surfaceIntersection.setPdf(pdf);

    Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleSpectrum;
    _albedo->evaluate(uvw, &sampleSpectrum);

    return sampleSpectrum * constant::INV_PI;
}

real LambertianDiffuse::evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const {
    const Vector3R normal = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R direction = surfaceIntersection.wo();

    return direction.absDot(normal) * constant::INV_PI;
}

} // namespace cadise