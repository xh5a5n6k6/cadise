#include "core/material/material.h"

#include "core/bxdf/bxdf.h"

namespace cadise {

Vector3F Material::evaluateBSDF(Vector3F inDirection, Vector3F outDirection, SurfaceInfo &surfaceInfo) {
    Vector3F f(0.0f);
    for (int i = 0; i < _bsdf.size(); i++) {
        f += _bsdf[i]->evaluate(inDirection, outDirection, surfaceInfo);
    }

    return f;
}

Vector3F Material::evaluateSampleBSDF(Vector3F inDirection, Vector3F &outDirection, SurfaceInfo &surfaceInfo) {
    Vector3F f(0.0f);
    for (int i = 0; i < _bsdf.size(); i++) {
        f += _bsdf[i]->evaluateSample(inDirection, outDirection, surfaceInfo);
    }

    return f;
}


void Material::_addBxDF(std::shared_ptr<BxDF> bxdf) {
    _bsdf.push_back(bxdf);
}

} // namespace cadise