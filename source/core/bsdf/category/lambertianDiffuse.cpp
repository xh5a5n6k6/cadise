#include "core/bsdf/category/lambertianDiffuse.h"

#include "core/integral-tool/hemisphere.h"
#include "core/surfaceIntersection.h"
#include "core/texture/category/constantTexture.h"
#include "core/texture/texture.h"
#include "fundamental/assertion.h"
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

    CADISE_ASSERT(albedo);
}

Spectrum LambertianDiffuse::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    const Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleSpectrum;
    _albedo->evaluate(uvw, &sampleSpectrum);

    return sampleSpectrum * constant::INV_PI;
}

Spectrum LambertianDiffuse::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    const Vector3R normal = surfaceIntersection.surfaceInfo().shadingNormal();

    // build N's coordinate system
    const Vector3R zAxis(normal);
    Vector3R xAxis;
    Vector3R yAxis;
    math::buildCoordinateSystem(zAxis, &xAxis, &yAxis);

    Vector3R sampleDirection;
    hemisphere::cosineWeightedSampling({random::nextReal(), random::nextReal()}, &sampleDirection);

    // transform local space to N's space
    Vector3R outDirection = xAxis * sampleDirection.x() + 
                            yAxis * sampleDirection.y() + 
                            zAxis * sampleDirection.z();
    outDirection = outDirection.normalize();

    const real pdf = outDirection.absDot(zAxis) * constant::INV_PI;

    surfaceIntersection.setWo(outDirection);
    surfaceIntersection.setPdf(pdf);

    const Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleSpectrum;
    _albedo->evaluate(uvw, &sampleSpectrum);

    return sampleSpectrum * constant::INV_PI;
}

real LambertianDiffuse::evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const {
    const Vector3R normal    = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R direction = surfaceIntersection.wo();

    return direction.absDot(normal) * constant::INV_PI;
}

} // namespace cadise