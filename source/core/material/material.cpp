#include "core/material/material.h"

#include "core/bxdf/bxdf.h"

#include "math/vector.h"

namespace cadise {

Vector3 Material::evaluateBSDF(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) {
    Vector3 f;
    for (int i = 0; i < _bsdf.size(); i++) {
        f += _bsdf[i]->evaluate(inDirection, outDirection, surfaceInfo);
    }

    return f;
}

Vector3 Material::evaluateSampleBSDF(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) {
    Vector3 f;
    for (int i = 0; i < _bsdf.size(); i++) {
        f += _bsdf[i]->evaluateSample(inDirection, outDirection, surfaceInfo);
    }

    return f;
}


void Material::_addBxDF(std::shared_ptr<BxDF> bxdf) {
    _bsdf.push_back(bxdf);
}

} // namespace cadise