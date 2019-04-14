#pragma once

#include "core/bxdf/bxdf.h"

#include "core/surfaceInfo.h"

namespace cadise {

class Material {
public:
    virtual Vector3 evaluateBSDF(Vector3 &inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo);

protected:
    virtual void _addBxDF(std::shared_ptr<BxDF> bxdf);

private:
    std::vector<std::shared_ptr<BxDF> > _bsdf;
};

} // namespace cadise