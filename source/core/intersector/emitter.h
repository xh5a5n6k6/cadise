#pragma once

#include "core/intersector/intersector.h"

#include "core/light/areaLight.h"

namespace cadise {

class Material;

class Emitter : public Intersector {
public:
    Emitter(AreaLight areaLight, std::shared_ptr<Material> material);

    bool isIntersecting(Ray &ray, Intersection &intersection) override;
    bool isOccluded(Ray &ray) override;
    RGBColor emittance(Vector3F direction) override;

    Vector3F evaluateBSDF(Vector3F inDirection, Vector3F outDirection, SurfaceInfo &surfaceInfo) override;
    Vector3F evaluateSampleBSDF(Vector3F inDirection, Vector3F &outDirection, SurfaceInfo &surfaceInfo) override;

private:
    AreaLight _areaLight;
    std::shared_ptr<Material> _material;

    std::shared_ptr<Intersector> _self;
};

} // namespace cadise