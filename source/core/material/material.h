#pragma once

#include <memory>
#include <vector>

namespace cadise {

class BxDF;
class SurfaceInfo;
class Vector3;

class Material {
public:
    virtual Vector3 evaluateBSDF(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo);
    virtual Vector3 evaluateSampleBSDF(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo);

protected:
    virtual void _addBxDF(std::shared_ptr<BxDF> bxdf);

    std::vector<std::shared_ptr<BxDF> > _bsdf;
};

} // namespace cadise