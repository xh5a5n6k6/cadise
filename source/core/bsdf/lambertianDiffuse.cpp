#include "core/bsdf/lambertianDiffuse.h"

#include "core/sampling/sample.h"
#include "core/surfaceIntersection.h"

#include "math/constant.h"
#include "math/math.h"
#include "math/random.h"

namespace cadise {

LambertianDiffuse::LambertianDiffuse(const Vector3R albedo) :
    _albedo(albedo) {
}

Vector3R LambertianDiffuse::evaluate(const SurfaceIntersection surfaceIntersection) const {
    return _albedo * constant::INV_PI;
}

Vector3R LambertianDiffuse::evaluateSample(SurfaceIntersection &surfaceIntersection) const {
    Vector3R x;
    Vector3R y;
    Vector3R z(surfaceIntersection.surfaceGeometryInfo().normal());
    math::buildCoordinateSystem(z, x, y);
    
    //Vector3R sampleDir = sample::uniformHemisphere(random::get2D());
    Vector3R sampleDir = sample::cosineWeightedHemisphere(random::get2D());
    Vector3R outDirection = x * sampleDir.x() + y * sampleDir.y() + z * sampleDir.z();
    outDirection = outDirection.normalize();

    //real pdf = constant::INV_TWO_PI;
    real pdf = outDirection.absDot(z) * constant::INV_PI;

    surfaceIntersection.setWo(outDirection);
    surfaceIntersection.setPdf(pdf);

    return _albedo * constant::INV_PI;
}

} // namespace cadise