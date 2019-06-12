#include "core/intersector/primitive.h"

#include "core/color.h"
#include "core/intersection.h"
#include "core/material/material.h"
#include "core/shape/shape.h"

namespace cadise {

Primitive::Primitive(const std::shared_ptr<Shape> shape, const std::shared_ptr<Material> material) :
    _shape(shape), _material(material) {
    _self = std::make_shared<Primitive>(*this);
}

AABB3R Primitive::bound() const {
    return _shape->bound();
}

bool Primitive::isIntersecting(Ray &ray, Intersection &intersection) const {
    SurfaceInfo surfaceInfo;
    bool result = _shape->isIntersecting(ray, surfaceInfo);
    if (result) {
        intersection.setSurfaceInfo(surfaceInfo);
        intersection.setIntersector(_self);
    }

    return result;
}

bool Primitive::isOccluded(Ray &ray) const {
    return _shape->isOccluded(ray);
}

RGBColor Primitive::emittance(const Vector3R direction) const {
    return RGBColor(0.0_r, 0.0_r, 0.0_r);
}

Vector3R Primitive::evaluateBSDF(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const {
    return _material->evaluateBSDF(inDirection, outDirection, surfaceInfo);
}

Vector3R Primitive::evaluateSampleBSDF(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo) const {
    return _material->evaluateSampleBSDF(inDirection, outDirection, surfaceInfo);
}

} // namespace cadise