#pragma once

#include "math/vector.h"

#include <memory>
#include <vector>

namespace cadise {

class BxDF;
class SurfaceInfo;

class Material {
public:
    virtual Vector3R evaluateBSDF(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const;
    virtual Vector3R evaluateSampleBSDF(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo) const;

protected:
    virtual void _addBxDF(const std::shared_ptr<BxDF> bxdf);

    std::vector<std::shared_ptr<BxDF> > _bsdf;
};

} // namespace cadise