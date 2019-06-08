#pragma once

#include "core/intersector/intersector.h"

#include "core/light/areaLight.h"

namespace cadise {

class Material;

class Emitter : public Intersector {
public:
    Emitter(AreaLight areaLight, std::shared_ptr<Material> material);

    AABB3R bound() override;

    bool isIntersecting(Ray &ray, Intersection &intersection) override;
    bool isOccluded(Ray &ray) override;

    RGBColor emittance(Vector3R direction) override;

    Vector3R evaluateBSDF(Vector3R inDirection, Vector3R outDirection, SurfaceInfo &surfaceInfo) override;
    Vector3R evaluateSampleBSDF(Vector3R inDirection, Vector3R &outDirection, SurfaceInfo &surfaceInfo) override;

private:
    AreaLight _areaLight;
    std::shared_ptr<Material> _material;

    std::shared_ptr<Intersector> _self;
};

} // namespace cadise