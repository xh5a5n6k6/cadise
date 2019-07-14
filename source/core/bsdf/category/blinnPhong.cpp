#include "core/bsdf/category/blinnPhong.h"

#include "core/surfaceIntersection.h"

#include <algorithm>
#include <cmath>

namespace cadise {

BlinnPhong::BlinnPhong(const real exponent) :
    BSDF(BSDFType(BxDF_Type::GLOSSY_REFLECTION)),
    _exponent(exponent) {
}

// inDirection  : eye direction
// outDirection : light direction
Vector3R BlinnPhong::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    real brdfFactor = std::max(surfaceIntersection.surfaceGeometryInfo().normal().dot(surfaceIntersection.wi()), 0.0_r);

    // H : half vector
    Vector3R H = (surfaceIntersection.wi() + surfaceIntersection.wo()).normalize();
    real NdotH = std::max(H.dot(surfaceIntersection.surfaceGeometryInfo().normal()), 0.0_r);
    real specular = std::pow(NdotH, _exponent) * brdfFactor;
	
    return Vector3R(specular, specular, specular);
}

Vector3R BlinnPhong::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    return Vector3R(0.0_r);
}

} // namespace cadise