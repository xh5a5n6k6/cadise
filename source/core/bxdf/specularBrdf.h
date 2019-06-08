#pragma once

#include "core/bxdf/bxdf.h"

#include "math/vector.h"

namespace cadise {

class SpecularBRDF : public BxDF {
public:
    SpecularBRDF(Vector3R albedo);

    Vector3R evaluate(Vector3R inDirection, Vector3R outDirection, SurfaceInfo &surfaceInfo) override;
    Vector3R evaluateSample(Vector3R inDirection, Vector3R &outDirection, SurfaceInfo &surfaceInfo) override;

private:
    Vector3R _albedo;
};

} // namespace cadise