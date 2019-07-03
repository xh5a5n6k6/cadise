#include "core/bxdf/blinnPhongBrdf.h"

#include "core/surfaceInfo.h"

#include <algorithm>
#include <cmath>

namespace cadise {

BlinnPhongBRDF::BlinnPhongBRDF(const real exponent) :
    _exponent(exponent) {
}

// inDirection  : light direction
// outDirection : eye direction
Vector3R BlinnPhongBRDF::evaluate(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const {
    real brdfFactor = std::max(surfaceInfo.normal().dot(outDirection), 0.0_r);

    // H : half vector
    Vector3R H = (inDirection + outDirection).normalize();
    real NdotH = std::max(H.dot(surfaceInfo.normal()), 0.0_r);
    real specular = std::pow(NdotH, _exponent) * brdfFactor;
	
    return Vector3R(specular, specular, specular);
}

Vector3R BlinnPhongBRDF::evaluateSample(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo, real &pdf) const {
    return Vector3R(0.0_r, 0.0_r, 0.0_r);
}

} // namespace cadise