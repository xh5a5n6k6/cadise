#include "core/intersector/emitter.h"

#include "core/color.h"
#include "core/intersection.h"
#include "core/material/material.h"

namespace cadise {

Emitter::Emitter(const AreaLight areaLight, const std::shared_ptr<Material> material) :
    _areaLight(areaLight), _material(material) {
    _self = std::make_shared<Emitter>(*this);
}

AABB3R Emitter::bound() const {
    return _areaLight.shape()->bound();
}

bool Emitter::isIntersecting(Ray &ray, Intersection &intersection) const {
    SurfaceInfo surfaceInfo;
    bool result = _areaLight.shape()->isIntersecting(ray, surfaceInfo);
    if (result) {
        intersection.setSurfaceInfo(surfaceInfo);
        intersection.setIntersector(_self);
    }

    return result;
}

bool Emitter::isOccluded(Ray &ray) const {
    return _areaLight.shape()->isOccluded(ray);
}

RGBColor Emitter::emittance(const Vector3R direction) const {
    return _areaLight.color();
    //RGBColor color = _areaLight.color();
    //return RGBColor(color.r() / 50.0_r, color.g() / 50.0_r, color.b() / 50.0_r);
}

Vector3R Emitter::evaluateBSDF(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const {
    return _material->evaluateBSDF(inDirection, outDirection, surfaceInfo);
}

Vector3R Emitter::evaluateSampleBSDF(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo, real &pdf) const {
    return _material->evaluateSampleBSDF(inDirection, outDirection, surfaceInfo, pdf);
}

} // namespace cadise