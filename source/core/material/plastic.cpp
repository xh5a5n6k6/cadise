#include "core/material/plastic.h"

#include "core/bxdf/lambertianBrdf.h"
#include "core/bxdf/blinnPhongBrdf.h"
#include "core/surfaceInfo.h"

namespace cadise {

Plastic::Plastic(Vector3 albedo, float exponent, float diffuseRatio) :
    _diffuseRatio(diffuseRatio) {
    _addBxDF(std::make_shared<LambertianBRDF>(albedo));
    _addBxDF(std::make_shared<BlinnPhongBRDF>(exponent));
}

Vector3 Plastic::evaluateBSDF(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) {
    return _diffuseRatio * _bsdf.at(0)->evaluate(inDirection, outDirection, surfaceInfo) +
           (1.0 - _diffuseRatio) * _bsdf.at(1)->evaluate(inDirection, outDirection, surfaceInfo);
}

Vector3 Plastic::evaluateSampleBSDF(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3(0.0f, 0.0f, 0.0f);
}

} // namespace cadise