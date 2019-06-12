#pragma once

#include "core/bxdf/bxdf.h"

#include "math/vector.h"

namespace cadise {

class SpecularBRDF : public BxDF {
public:
    SpecularBRDF(const Vector3R albedo);

    Vector3R evaluate(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const override;
    Vector3R evaluateSample(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo) const override;

private:
    Vector3R _albedo;
};

} // namespace cadise