#pragma once

#include "core/bxdf/bxdf.h"

#include "math/algebra.h"

namespace cadise {

class SpecularBRDF : public BxDF {
public:
    Vector3 evaluate(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) override;
    Vector3 evaluateSample(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) override;
};

} // namespace cadise