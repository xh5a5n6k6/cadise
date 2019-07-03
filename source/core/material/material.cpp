#include "core/material/material.h"

#include "core/bxdf/bxdf.h"
#include "core/surfaceInfo.h"

namespace cadise {

Vector3R Material::evaluateBSDF(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const {
    Vector3R f(0.0_r);
    for (uint64 i = 0; i < _bsdf.size(); i++) {
        f += _bsdf[i]->evaluate(inDirection, outDirection, surfaceInfo);
    }

    return f;
}

Vector3R Material::evaluateSampleBSDF(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo, real &pdf) const {
    Vector3R f(0.0_r);
    for (uint64 i = 0; i < _bsdf.size(); i++) {
        f += _bsdf[i]->evaluateSample(inDirection, outDirection, surfaceInfo, pdf);
    }

    return f;
}

void Material::_addBxDF(const std::shared_ptr<BxDF> bxdf) {
    _bsdf.push_back(bxdf);
}

} // namespace cadise