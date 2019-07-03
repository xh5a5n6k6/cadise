#pragma once

#include "core/intersector/intersector.h"

#include <memory>

namespace cadise {

class Material;
class Shape;

class Primitive : public Intersector {
public:
    Primitive(const std::shared_ptr<Shape> shape, const std::shared_ptr<Material> material);
	
    AABB3R bound() const override;

    bool isIntersecting(Ray &ray, Intersection &intersection) const override;
    bool isOccluded(Ray &ray) const override;

    RGBColor emittance(const Vector3R direction) const override;

    Vector3R evaluateBSDF(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const override;
    Vector3R evaluateSampleBSDF(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo, real &pdf) const override;

private:
    std::shared_ptr<Shape> _shape;
    std::shared_ptr<Material> _material;

    std::shared_ptr<Intersector> _self;
};

} // namespace cadise