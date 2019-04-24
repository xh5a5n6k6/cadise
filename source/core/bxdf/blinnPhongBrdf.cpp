#include "core/bxdf/blinnPhongBrdf.h"

#include "core/surfaceInfo.h"

#include "math/constant.h"

#include <algorithm>

namespace cadise {

BlinnPhongBRDF::BlinnPhongBRDF(float exponent) :
    _exponent(exponent) {
}

// inDirection  : light direction
// outDirection : eye direction
Vector3 BlinnPhongBRDF::evaluate(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) {
    float brdfFactor = std::max(Dot(surfaceInfo.hitNormal(), outDirection), 0.0f);

    // h : half vector
    Vector3 h = (inDirection + outDirection).normalize();
    float cosTheta = std::max(Dot(h, surfaceInfo.hitNormal()), 0.0f);
    float specular = std::powf(cosTheta, _exponent) * brdfFactor;
	
    return Vector3(specular, specular, specular);
}

Vector3 BlinnPhongBRDF::evaluateSample(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3(0.0f, 0.0f, 0.0f);
}

} // namespace cadise