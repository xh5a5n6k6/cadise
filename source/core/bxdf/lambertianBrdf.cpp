#include "core/bxdf/lambertianBrdf.h"

#include "core/sampling/sample.h"
#include "core/surfaceInfo.h"

#include "math/constant.h"
#include "math/math.h"
#include "math/random.h"

namespace cadise {

LambertianBRDF::LambertianBRDF(const Vector3R albedo) :
    _albedo(albedo) {
}

Vector3R LambertianBRDF::evaluate(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const {
    return _albedo * constant::INV_PI;
}

Vector3R LambertianBRDF::evaluateSample(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo, real &pdf) const {
    Vector3R x;
    Vector3R y;
    Vector3R z(surfaceInfo.normal());
    math::buildCoordinateSystem(z, x, y);
    
    //Vector3R sampleDir = sample::uniformHemisphere(random::get2D());
    Vector3R sampleDir = sample::cosineWeightedHemisphere(random::get2D());
    outDirection = x * sampleDir.x() + y * sampleDir.y() + z * sampleDir.z();
    outDirection = outDirection.normalize();

    //pdf = constant::INV_TWO_PI;
    pdf = outDirection.absDot(z) * constant::INV_PI;

    return _albedo * constant::INV_PI;
}

} // namespace cadise