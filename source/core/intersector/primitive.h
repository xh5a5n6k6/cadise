#pragma once

#include "core/intersector/intersector.h"

#include <memory>

namespace cadise {

class Material;
class Shape;

class Primitive : public Intersector {
public:
    Primitive(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material);
	
    bool isIntersecting(Ray &ray, Intersection &intersection) override;
    bool isOccluded(Ray &ray) override;
    RGBColor emittance(Vector3F direction) override;

    Vector3F evaluateBSDF(Vector3F inDirection, Vector3F outDirection, SurfaceInfo &surfaceInfo) override;
    Vector3F evaluateSampleBSDF(Vector3F inDirection, Vector3F &outDirection, SurfaceInfo &surfaceInfo) override;

private:
    std::shared_ptr<Shape> _shape;
    std::shared_ptr<Material> _material;

    std::shared_ptr<Intersector> _self;
};

} // namespace cadise