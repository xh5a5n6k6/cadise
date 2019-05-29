#include "core/bxdf/blinnPhongBrdf.h"

#include "core/surfaceInfo.h"

#include <algorithm>
#include <cmath>

namespace cadise {

BlinnPhongBRDF::BlinnPhongBRDF(float exponent) :
    _exponent(exponent) {
}

// inDirection  : light direction
// outDirection : eye direction
Vector3F BlinnPhongBRDF::evaluate(Vector3F inDirection, Vector3F outDirection, SurfaceInfo &surfaceInfo) {
    float brdfFactor = std::max(surfaceInfo.normal().dot(outDirection), 0.0f);

    // h : half vector
    Vector3F h = (inDirection + outDirection).normalize();
    float cosTheta = std::max(h.dot(surfaceInfo.normal()), 0.0f);
    float specular = std::powf(cosTheta, _exponent) * brdfFactor;
	
    return Vector3F(specular, specular, specular);
}

Vector3F BlinnPhongBRDF::evaluateSample(Vector3F inDirection, Vector3F &outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3F(0.0f, 0.0f, 0.0f);
}

} // namespace cadise