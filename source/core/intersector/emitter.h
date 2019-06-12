#pragma once

#include "core/intersector/intersector.h"

#include "core/light/areaLight.h"

namespace cadise {

class Material;

class Emitter : public Intersector {
public:
    Emitter(const AreaLight areaLight, const std::shared_ptr<Material> material);

    AABB3R bound() const override;

    bool isIntersecting(Ray &ray, Intersection &intersection) const override;
    bool isOccluded(Ray &ray) const override;

    RGBColor emittance(const Vector3R direction) const override;

    Vector3R evaluateBSDF(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const override;
    Vector3R evaluateSampleBSDF(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo) const override;

private:
    AreaLight _areaLight;
    std::shared_ptr<Material> _material;

    std::shared_ptr<Intersector> _self;
};

} // namespace cadise