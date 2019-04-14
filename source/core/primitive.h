#pragma once

#include "core/intersector.h"
#include "core/shape.h"
#include "core/material.h"
#include "core/color.h"

namespace cadise {

class Primitive : public Intersector {
public:
    Primitive();
    Primitive(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material);
	
    virtual bool isIntersecting(Ray &ray, Intersection &intersection) override;
    virtual bool isOccluded(Ray &ray) override;
    virtual RGBColor emittance() override;

    Vector3 evaluateBSDF(Vector3 &inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo);

private:
    std::shared_ptr<Shape> _shape;
    std::shared_ptr<Material> _material;
};

} // namespace cadise