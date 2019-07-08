#include "core/surfaceIntersection.h"

namespace cadise {

SurfaceIntersection::SurfaceIntersection() :
    _primitiveInfo(), 
    _geometryInfo(), 
    _shadingInfo(), 
    _wi(), 
    _wo(), 
    _pdf(0.0_r) {
}

PrimitiveInfo SurfaceIntersection::primitiveInfo() const {
    return _primitiveInfo;
}

SurfaceGeometryInfo SurfaceIntersection::surfaceGeometryInfo() const {
    return _geometryInfo;
}

SurfaceShadingInfo SurfaceIntersection::surfaceShadingInfo() const {
    return _shadingInfo;
}

Vector3R SurfaceIntersection::wi() const {
    return _wi;
}

Vector3R SurfaceIntersection::wo() const {
    return _wo;
}

real SurfaceIntersection::pdf() const {
    return _pdf;
}

void SurfaceIntersection::setPrimitiveInfo(const PrimitiveInfo& primitiveInfo) {
    _primitiveInfo = primitiveInfo;
}

void SurfaceIntersection::setSurfaceGeometryInfo(const SurfaceGeometryInfo& geometryInfo) {
    _geometryInfo = geometryInfo;
}

void SurfaceIntersection::setSurfaceShadingInfo(const SurfaceShadingInfo& shadingInfo) {
    _shadingInfo = shadingInfo;
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