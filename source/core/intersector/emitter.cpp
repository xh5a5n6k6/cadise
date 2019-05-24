#include "core/intersector/emitter.h"

#include "core/color.h"
#include "core/intersection.h"
#include "core/material/material.h"

namespace cadise {

Emitter::Emitter(AreaLight areaLight, std::shared_ptr<Material> material) :
    _areaLight(areaLight), _material(material) {
    _self = std::make_shared<Emitter>(*this);
}

bool Emitter::isIntersecting(Ray &ray, Intersection &intersection) {
    SurfaceInfo surfaceInfo;
    bool result = _areaLight.shape()->isIntersecting(ray, surfaceInfo);
    if (result) {
        intersection.setSurfaceInfo(surfaceInfo);
        intersection.setIntersector(_self);
    }

    return result;
}

bool Emitter::isOccluded(Ray &ray) {
    return _areaLight.shape()->isOccluded(ray);
}

RGBColor Emitter::emittance(Vector3F direction) {
    return _areaLight.color();
}

Vector3F Emitter::evaluateBSDF(Vector3F inDirection, Vector3F outDirection, SurfaceInfo &surfaceInfo) {
    return _material->evaluateBSDF(inDirection, outDirection, surfaceInfo);
}

Vector3F Emitter::evaluateSampleBSDF(Vector3F inDirection, Vector3F &outDirection, SurfaceInfo &surfaceInfo) {
    return _material->evaluateSampleBSDF(inDirection, outDirection, surfaceInfo);
}

} // namespace cadise