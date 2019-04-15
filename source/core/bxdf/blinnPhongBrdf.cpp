#include "core/bxdf/blinnPhongBrdf.h"

#include "core/surfaceInfo.h"

namespace cadise {

BlinnPhongBRDF::BlinnPhongBRDF(float exponent) :
    _exponent(exponent) {
    _brdfFactor = (_exponent + 2.0f) * (_exponent + 4.0f) * CADISE_INV_EIGHT_PI / (std::exp2f(-_exponent / 2.0f) + _exponent);
}

// inDirection  : light direction
// outDirection : eye direction
Vector3 BlinnPhongBRDF::evaluate(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) {
    Vector3 h = (inDirection + outDirection).normalize();
    float cosTheta = std::max(Dot(h, surfaceInfo.hitNormal()), 0.0f);
    float specular = std::powf(cosTheta, _exponent) * _brdfFactor;
	
    return Vector3(specular, specular, specular);
}

Vector3 BlinnPhongBRDF::evaluateSample(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3(0.0f, 0.0f, 0.0f);
}

} // namespace cadise