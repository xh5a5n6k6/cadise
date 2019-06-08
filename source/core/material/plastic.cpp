#include "core/material/plastic.h"

#include "core/bxdf/lambertianBrdf.h"
#include "core/bxdf/blinnPhongBrdf.h"
#include "core/surfaceInfo.h"

namespace cadise {

Plastic::Plastic(Vector3R albedo, real exponent, real diffuseRatio) :
    _diffuseRatio(diffuseRatio) {
    _addBxDF(std::make_shared<LambertianBRDF>(albedo));
    _addBxDF(std::make_shared<BlinnPhongBRDF>(exponent));
}

Vector3R Plastic::evaluateBSDF(Vector3R inDirection, Vector3R outDirection, SurfaceInfo &surfaceInfo) {
    return _diffuseRatio * _bsdf.at(0)->evaluate(inDirection, outDirection, surfaceInfo) +
           (1.0_r - _diffuseRatio) * _bsdf.at(1)->evaluate(inDirection, outDirection, surfaceInfo);
}

Vector3R Plastic::evaluateSampleBSDF(Vector3R inDirection, Vector3R &outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3R(0.0_r, 0.0_r, 0.0_r);
}

} // namespace cadise