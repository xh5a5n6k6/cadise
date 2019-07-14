#include "core/bsdf/category/lambertianDiffuse.h"

#include "core/sampling/sample.h"
#include "core/surfaceIntersection.h"

#include "math/constant.h"
#include "math/math.h"
#include "math/random.h"

namespace cadise {

LambertianDiffuse::LambertianDiffuse(const Vector3R& albedo) :
    BSDF(BSDFType(BxDF_Type::DIFFUSE_REFLECTION)),
    _albedo(albedo) {
}

Vector3R LambertianDiffuse::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    return _albedo * constant::INV_PI;
}

Vector3R LambertianDiffuse::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    Vector3R xAxis;
    Vector3R yAxis;
    Vector3R zAxis(surfaceIntersection.surfaceGeometryInfo().normal());
    math::buildCoordinateSystem(zAxis, xAxis, yAxis);
    
    //Vector3R sampleDir = sample::uniformHemisphere(random::get2D());
    Vector3R sampleDir = sample::cosineWeightedHemisphere(random::get2D());
    Vector3R outDirection = xAxis * sampleDir.x() + yAxis * sampleDir.y() + zAxis * sampleDir.z();
    outDirection = outDirection.normalize();

    //real pdf = constant::INV_TWO_PI;
    real pdf = outDirection.absDot(zAxis) * constant::INV_PI;

    surfaceIntersection.setWo(outDirection);
    surfaceIntersection.setPdf(pdf);

    return _albedo * constant::INV_PI;
}

} // namespace cadise