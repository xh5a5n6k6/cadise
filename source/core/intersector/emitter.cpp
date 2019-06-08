#include "core/intersector/emitter.h"

#include "core/color.h"
#include "core/intersection.h"
#include "core/material/material.h"

namespace cadise {

Emitter::Emitter(AreaLight areaLight, std::shared_ptr<Material> material) :
    _areaLight(areaLight), _material(material) {
    _self = std::make_shared<Emitter>(*this);
}

AABB3R Emitter::bound() {
    return _areaLight.shape()->bound();
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

RGBColor Emitter::emittance(Vector3R direction) {
    return _areaLight.color();
}

Vector3R Emitter::evaluateBSDF(Vector3R inDirection, Vector3R outDirection, SurfaceInfo &surfaceInfo) {
    return _material->evaluateBSDF(inDirection, outDirection, surfaceInfo);
}

Vector3R Emitter::evaluateSampleBSDF(Vector3R inDirection, Vector3R &outDirection, SurfaceInfo &surfaceInfo) {
    return _material->evaluateSampleBSDF(inDirection, outDirection, surfaceInfo);
}

} // namespace cadise