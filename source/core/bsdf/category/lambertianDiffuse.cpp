#include "core/bsdf/category/lambertianDiffuse.h"

#include "core/sampling/sample.h"
#include "core/surfaceIntersection.h"
#include "core/texture/texture.h"

#include "math/constant.h"
#include "math/math.h"
#include "math/random.h"

namespace cadise {

LambertianDiffuse::LambertianDiffuse(const std::shared_ptr<Texture<Spectrum>>& albedo) :
    Bsdf(BsdfType(BxdfType::DIFFUSE_REFLECTION)),
    _albedo(albedo) {
}

Spectrum LambertianDiffuse::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
    return _albedo->evaluate(uvw) * constant::INV_PI;
}

Spectrum LambertianDiffuse::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    Vector3R xAxis;
    Vector3R yAxis;
    Vector3R zAxis(surfaceIntersection.surfaceInfo().shadingNormal());
    math::buildCoordinateSystem(zAxis, xAxis, yAxis);
    
    //Vector3R sampleDir = sample::uniformHemisphere(random::get2D());
    Vector3R sampleDir = sample::cosineWeightedHemisphere(random::get2D());
    Vector3R outDirection = xAxis * sampleDir.x() + yAxis * sampleDir.y() + zAxis * sampleDir.z();
    outDirection = outDirection.normalize();

    //real pdf = constant::INV_TWO_PI;
    real pdf = outDirection.absDot(zAxis) * constant::INV_PI;

    surfaceIntersection.setWo(outDirection);
    surfaceIntersection.setPdf(pdf);

    Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
    return _albedo->evaluate(uvw) * constant::INV_PI;
}

} // namespace cadise