#pragma once

#include "core/basicHeader.h"
#include "core/intersector.h"

namespace cadise {

class Material;
class Shape;
class SurfaceInfo;
class Vector3;

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