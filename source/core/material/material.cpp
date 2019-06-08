#include "core/material/material.h"

#include "core/bxdf/bxdf.h"

namespace cadise {

Vector3R Material::evaluateBSDF(Vector3R inDirection, Vector3R outDirection, SurfaceInfo &surfaceInfo) {
    Vector3R f(0.0_r);
    for (uint64 i = 0; i < _bsdf.size(); i++) {
        f += _bsdf[i]->evaluate(inDirection, outDirection, surfaceInfo);
    }

    return f;
}

Vector3R Material::evaluateSampleBSDF(Vector3R inDirection, Vector3R &outDirection, SurfaceInfo &surfaceInfo) {
    Vector3R f(0.0_r);
    for (uint64 i = 0; i < _bsdf.size(); i++) {
        f += _bsdf[i]->evaluateSample(inDirection, outDirection, surfaceInfo);
    }

    return f;
}


void Material::_addBxDF(std::shared_ptr<BxDF> bxdf) {
    _bsdf.push_back(bxdf);
}

} // namespace cadise