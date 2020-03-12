#include "core/surfaceInfo.h"

namespace cadise {

SurfaceInfo::SurfaceInfo() :
    _position(0.0_r),
    _geometryNormal(0.0_r),
    _shadingNormal(0.0_r),
    _uvw(0.0_r) {
}

const Vector3R& SurfaceInfo::position() const {
    return _position;
}

const Vector3R& SurfaceInfo::geometryNormal() const {
    return _geometryNormal;
}

const Vector3R& SurfaceInfo::shadingNormal() const {
    return _shadingNormal;
}

const Vector3R& SurfaceInfo::uvw() const {
    return _uvw;
}

void SurfaceInfo::setPosition(const Vector3R& position) {
    _position = position;
}

void SurfaceInfo::setGeometryNormal(const Vector3R& geometryNormal) {
    _geometryNormal = geometryNormal;
}

void SurfaceInfo::setShadingNormal(const Vector3R& shadingNormal) {
    _shadingNormal = shadingNormal;
}

void SurfaceInfo::setUvw(const Vector3R& uvw) {
    _uvw = uvw;
}

} // namespace cadise