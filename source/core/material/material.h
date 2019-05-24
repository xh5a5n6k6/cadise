#pragma once

#include "math/vector.h"

#include <memory>
#include <vector>

namespace cadise {

class BxDF;
class SurfaceInfo;

class Material {
public:
    virtual Vector3F evaluateBSDF(Vector3F inDirection, Vector3F outDirection, SurfaceInfo &surfaceInfo);
    virtual Vector3F evaluateSampleBSDF(Vector3F inDirection, Vector3F &outDirection, SurfaceInfo &surfaceInfo);

protected:
    virtual void _addBxDF(std::shared_ptr<BxDF> bxdf);

    std::vector<std::shared_ptr<BxDF> > _bsdf;
};

} // namespace cadise