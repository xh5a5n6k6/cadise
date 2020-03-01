#pragma once

#include "core/light/category/areaLight.h"

#include <memory>

namespace cadise {

class Primitive;
template<typename T>
class Texture;

class SingleAreaLight : public AreaLight {
public:
    SingleAreaLight(const Primitive* const primitive,
                    const Spectrum& color, 
                    const real watt, 
                    const bool isBackFaceEmit);

    Spectrum emittance(const SurfaceIntersection& emitIntersection) const override;

    void evaluateDirectSample(DirectLightSample* const out_sample) const override;
    real evaluateDirectPdfW(
        const SurfaceIntersection& emitIntersection, 
        const Vector3R&            targetPosition) const override;

    real approximatedFlux() const override;

    void setEmitRadiance(const std::shared_ptr<Texture<Spectrum>>& emitRadiance);

private:
    const Primitive* _primitive;
    std::shared_ptr<Texture<Spectrum>> _emitRadiance;
};

} // namespace cadise