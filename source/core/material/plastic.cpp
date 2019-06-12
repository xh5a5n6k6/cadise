#include "core/material/plastic.h"

#include "core/bxdf/lambertianBrdf.h"
#include "core/bxdf/blinnPhongBrdf.h"
#include "core/surfaceInfo.h"

namespace cadise {

Plastic::Plastic(const Vector3R albedo, const real exponent, const real diffuseRatio) :
    _diffuseRatio(diffuseRatio) {
    _addBxDF(std::make_shared<LambertianBRDF>(albedo));
    _addBxDF(std::make_shared<BlinnPhongBRDF>(exponent));
}

Vector3R Plastic::evaluateBSDF(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const {
    return _diffuseRatio * _bsdf.at(0)->evaluate(inDirection, outDirection, surfaceInfo) +
           (1.0_r - _diffuseRatio) * _bsdf.at(1)->evaluate(inDirection, outDirection, surfaceInfo);
}

Vector3R Plastic::evaluateSampleBSDF(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo) const {
    return Vector3R(0.0_r, 0.0_r, 0.0_r);
}

} // namespace cadise