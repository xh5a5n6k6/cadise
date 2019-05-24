#include "core/intersector/primitive.h"

#include "core/color.h"
#include "core/intersection.h"
#include "core/material/material.h"
#include "core/shape/shape.h"

namespace cadise {

Primitive::Primitive(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material) :
    _shape(shape), _material(material) {
    _self = std::make_shared<Primitive>(*this);
}

bool Primitive::isIntersecting(Ray &ray, Intersection &intersection) {
    SurfaceInfo surfaceInfo;
    bool result = _shape->isIntersecting(ray, surfaceInfo);
    if (result) {
        intersection.setSurfaceInfo(surfaceInfo);
        intersection.setIntersector(_self);
    }

    return result;
}

bool Primitive::isOccluded(Ray &ray) {
    return _shape->isOccluded(ray);
}

RGBColor Primitive::emittance(Vector3F direction) {
    return RGBColor(0.0f, 0.0f, 0.0f);
}

Vector3F Primitive::evaluateBSDF(Vector3F inDirection, Vector3F outDirection, SurfaceInfo &surfaceInfo) {
    return _material->evaluateBSDF(inDirection, outDirection, surfaceInfo);
}

Vector3F Primitive::evaluateSampleBSDF(Vector3F inDirection, Vector3F &outDirection, SurfaceInfo &surfaceInfo) {
    return _material->evaluateSampleBSDF(inDirection, outDirection, surfaceInfo);
}

} // namespace cadise