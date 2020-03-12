#include "core/surfaceIntersection.h"

namespace cadise {

SurfaceIntersection::SurfaceIntersection() :
    _primitiveInfo(), 
    _surfaceInfo(),
    _wi(), 
    _wo(), 
    _pdf(0.0_r) {
}

SurfaceIntersection SurfaceIntersection::reverse() const {
    SurfaceIntersection intersection(*this);
    intersection.setWi(_wo);
    intersection.setWo(_wi);

    return intersection;
}

const PrimitiveInfo& SurfaceIntersection::primitiveInfo() const {
    return _primitiveInfo;
}

const SurfaceInfo& SurfaceIntersection::surfaceInfo() const {
    return _surfaceInfo;
}

const Vector3R& SurfaceIntersection::wi() const {
    return _wi;
}

const Vector3R& SurfaceIntersection::wo() const {
    return _wo;
}

real SurfaceIntersection::pdf() const {
    return _pdf;
}

void SurfaceIntersection::setPrimitiveInfo(const PrimitiveInfo& primitiveInfo) {
    _primitiveInfo = primitiveInfo;
}

void SurfaceIntersection::setSurfaceInfo(const SurfaceInfo& surfaceInfo) {
    _surfaceInfo = surfaceInfo;
}

void SurfaceIntersection::setWi(const Vector3R& wi) {
    _wi = wi;
}

void SurfaceIntersection::setWo(const Vector3R& wo) {
    _wo = wo;
}

void SurfaceIntersection::setPdf(const real pdf) {
    _pdf = pdf;
}

} // namespace cadise