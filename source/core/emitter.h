#pragma once

#include "core/intersector.h"

#include "core/light/areaLight.h"

namespace cadise {

class Material;

class Emitter : public Intersector {
public:
    Emitter(AreaLight areaLight, std::shared_ptr<Material> material);

    bool isIntersecting(Ray &ray, Intersection &intersection) override;
    bool isOccluded(Ray &ray) override;
    RGBColor emittance(Vector3 direction) override;

    Vector3 evaluateBSDF(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) override;
    Vector3 evaluateSampleBSDF(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) override;

private:
    AreaLight _areaLight;
    std::shared_ptr<Material> _material;

    std::shared_ptr<Intersector> _self;
};

} // namespace cadise