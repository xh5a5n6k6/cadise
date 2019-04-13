#include "core/bxdf/blinnPhongBrdf.h"

namespace cadise {

BlinnPhongBRDF::BlinnPhongBRDF(Vector3 albedo, float exponent, float diffuseRatio) :
	_albedo(albedo), _exponent(exponent), _diffuseRatio(diffuseRatio) {
	_brdfFactor = (_exponent + 2.0f) * (_exponent + 4.0f) * CADISE_INV_EIGHT_PI / (std::exp2f(-_exponent / 2.0f) + _exponent);
}

// inDirection  : light direction
// outDirection : eye direction
Vector3 BlinnPhongBRDF::evaluate(Vector3 &inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) {
	Vector3 diffuse = _diffuseRatio * _albedo;// *CADISE_INV_PI;
	
	Vector3 h = (inDirection + outDirection).normalize();
	float cosTheta = std::max(Dot(h, surfaceInfo.hitNormal()), 0.0f);
	float specular = (1.0f - _diffuseRatio) * std::powf(cosTheta, _exponent);// *_brdfFactor;
	
	return diffuse + Vector3(specular, specular, specular);
}

} // namespace cadise