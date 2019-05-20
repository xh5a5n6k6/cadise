#pragma once

#include "core/intersector/intersector.h"

#include <memory>

namespace cadise {

class Material;
class Shape;
class SurfaceInfo;
class Vector3;

class Primitive : public Intersector {
public:
    Primitive();
    Primitive(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material);
	
    bool isIntersecting(Ray &ray, Intersection &intersection) override;
    bool isOccluded(Ray &ray) override;
    RGBColor emittance(Vector3 direction) override;

    Vector3 evaluateBSDF(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) override;
    Vector3 evaluateSampleBSDF(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) override;

private:
    std::shared_ptr<Shape> _shape;
    std::shared_ptr<Material> _material;

    std::shared_ptr<Intersector> _self;
};

} // namespace cadise