#pragma once

#include "core/bxdf/bxdf.h"

#include "math/vector.h"

namespace cadise {

class LambertianBRDF : public BxDF {
public:
    LambertianBRDF(Vector3 albedo);

    Vector3 evaluate(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) override;
    Vector3 evaluateSample(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) override;

private:
    Vector3 _albedo;
};

} // namespace cadise