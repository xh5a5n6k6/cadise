#pragma once

#include "core/bxdf/bxdf.h"

#include "math/vector.h"

namespace cadise {

class SpecularBRDF : public BxDF {
public:
    SpecularBRDF(Vector3F albedo);

    Vector3F evaluate(Vector3F inDirection, Vector3F outDirection, SurfaceInfo &surfaceInfo) override;
    Vector3F evaluateSample(Vector3F inDirection, Vector3F &outDirection, SurfaceInfo &surfaceInfo) override;

private:
    Vector3F _albedo;
};

} // namespace cadise