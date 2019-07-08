#pragma once

#include "core/bsdf/bsdf.h"

namespace cadise {

class SpecularReflection : public BSDF {
public:
    SpecularReflection(const Vector3R& albedo);

    Vector3R evaluate(const SurfaceIntersection& surfaceIntersection) const override;
    Vector3R evaluateSample(SurfaceIntersection& surfaceIntersection) const override;

private:
    Vector3R _albedo;
};

} // namespace cadise