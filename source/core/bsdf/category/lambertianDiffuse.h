#pragma once

#include "core/bsdf/bsdf.h"

#include <memory>

namespace cadise {

template<typename T>
class Texture;

class LambertianDiffuse : public BSDF {
public:
    LambertianDiffuse(const std::shared_ptr<Texture<Spectrum>>& albedo);

    Spectrum evaluate(const SurfaceIntersection& surfaceIntersection) const override;
    Spectrum evaluateSample(SurfaceIntersection& surfaceIntersection) const override;

private:
    std::shared_ptr<Texture<Spectrum>> _albedo;
};

} // namespace cadise