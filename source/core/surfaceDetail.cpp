#include "core/surfaceDetail.h"

namespace cadise {

SurfaceDetail::SurfaceDetail() :
    _position(0.0_r),
    _geometryNormal(0.0_r),
    _shadingNormal(0.0_r),
    _uvw(0.0_r) {
}

const Vector3R& SurfaceDetail::position() const {
    return _position;
}

const Vector3R& SurfaceDetail::geometryNormal() const {
    return _geometryNormal;
}

const Vector3R& SurfaceDetail::shadingNormal() const {
    return _shadingNormal;
}

const Vector3R& SurfaceDetail::uvw() const {
    return _uvw;
}

void SurfaceDetail::setPosition(const Vector3R& position) {
    _position = position;
}

void SurfaceDetail::setGeometryNormal(const Vector3R& geometryNormal) {
    _geometryNormal = geometryNormal;
}

void SurfaceDetail::setShadingNormal(const Vector3R& shadingNormal) {
    _shadingNormal = shadingNormal;
}

void SurfaceDetail::setUvw(const Vector3R& uvw) {
    _uvw = uvw;
}

} // namespace cadise