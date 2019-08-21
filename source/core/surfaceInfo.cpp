#include "core/surfaceInfo.h"

namespace cadise {

SurfaceInfo::SurfaceInfo() :
    _point(0.0_r),
    _geometryNormal(0.0_r),
    _shadingNormal(0.0_r),
    _frontNormal(0.0_r),
    _uvw(0.0_r) {
}

Vector3R SurfaceInfo::point() const {
    return _point;
}

Vector3R SurfaceInfo::geometryNormal() const {
    return _geometryNormal;
}

Vector3R SurfaceInfo::shadingNormal() const {
    return _shadingNormal;
}

Vector3R SurfaceInfo::frontNormal() const {
    return _frontNormal;
}

Vector3R SurfaceInfo::uvw() const {
    return _uvw;
}

void SurfaceInfo::setPoint(const Vector3R& point) {
    _point = point;
}

void SurfaceInfo::setGeometryNormal(const Vector3R& geometryNormal) {
    _geometryNormal = geometryNormal;
}

void SurfaceInfo::setShadingNormal(const Vector3R& shadingNormal) {
    _shadingNormal = shadingNormal;
}

void SurfaceInfo::setFrontNormal(const Vector3R& frontNormal) {
    _frontNormal = frontNormal;
}

void SurfaceInfo::setUvw(const Vector3R& uvw) {
    _uvw = uvw;
}

} // namespace cadise