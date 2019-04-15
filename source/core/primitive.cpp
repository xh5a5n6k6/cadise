#include "core/primitive.h"

#include "core/color.h"
#include "core/intersection.h"
#include "core/material/material.h"
#include "core/shape/shape.h"

namespace cadise {

Primitive::Primitive() :
    Primitive(nullptr, nullptr) {
}

Primitive::Primitive(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material) :
    _shape(shape), _material(material) {
}

bool Primitive::isIntersecting(Ray &ray, Intersection &intersection) {
    SurfaceInfo surfaceInfo;
    bool result = _shape->isIntersecting(ray, surfaceInfo);
    if (result) {
        intersection.setSurfaceInfo(surfaceInfo);
        intersection.setPrimitive(*this);
    }

    return result;
}

bool Primitive::isOccluded(Ray &ray) {
    bool result = _shape->isOccluded(ray);

    return result;
}

RGBColor Primitive::emittance() {
    return RGBColor(0.0f, 0.0f, 0.0f);
}

Vector3 Primitive::evaluateBSDF(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) {
    return _material->evaluateBSDF(inDirection, outDirection, surfaceInfo);
}

Vector3 Primitive::evaluateSampleBSDF(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) {
    return _material->evaluateSampleBSDF(inDirection, outDirection, surfaceInfo);
}

} // namespace cadise