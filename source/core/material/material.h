#pragma once

#include "math/vector.h"

#include <memory>
#include <vector>

namespace cadise {

class BxDF;
class SurfaceInfo;

class Material {
public:
    virtual Vector3R evaluateBSDF(Vector3R inDirection, Vector3R outDirection, SurfaceInfo &surfaceInfo);
    virtual Vector3R evaluateSampleBSDF(Vector3R inDirection, Vector3R &outDirection, SurfaceInfo &surfaceInfo);

protected:
    virtual void _addBxDF(std::shared_ptr<BxDF> bxdf);

    std::vector<std::shared_ptr<BxDF> > _bsdf;
};

} // namespace cadise