#pragma once

#include "core/intersector/intersector.h"

#include <memory>

namespace cadise {

class Material;
class Shape;

class Primitive : public Intersector {
public:
    Primitive(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material);
	
    AABB3R bound() override;

    bool isIntersecting(Ray &ray, Intersection &intersection) override;
    bool isOccluded(Ray &ray) override;

    RGBColor emittance(Vector3R direction) override;

    Vector3R evaluateBSDF(Vector3R inDirection, Vector3R outDirection, SurfaceInfo &surfaceInfo) override;
    Vector3R evaluateSampleBSDF(Vector3R inDirection, Vector3R &outDirection, SurfaceInfo &surfaceInfo) override;

private:
    std::shared_ptr<Shape> _shape;
    std::shared_ptr<Material> _material;

    std::shared_ptr<Intersector> _self;
};

} // namespace cadise