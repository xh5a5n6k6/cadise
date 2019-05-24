#include "core/material/plastic.h"

#include "core/bxdf/lambertianBrdf.h"
#include "core/bxdf/blinnPhongBrdf.h"
#include "core/surfaceInfo.h"

namespace cadise {

Plastic::Plastic(Vector3F albedo, float exponent, float diffuseRatio) :
    _diffuseRatio(diffuseRatio) {
    _addBxDF(std::make_shared<LambertianBRDF>(albedo));
    _addBxDF(std::make_shared<BlinnPhongBRDF>(exponent));
}

Vector3F Plastic::evaluateBSDF(Vector3F inDirection, Vector3F outDirection, SurfaceInfo &surfaceInfo) {
    return _diffuseRatio * _bsdf.at(0)->evaluate(inDirection, outDirection, surfaceInfo) +
           (1.0f - _diffuseRatio) * _bsdf.at(1)->evaluate(inDirection, outDirection, surfaceInfo);
}

Vector3F Plastic::evaluateSampleBSDF(Vector3F inDirection, Vector3F &outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3F(0.0f, 0.0f, 0.0f);
}

} // namespace cadise