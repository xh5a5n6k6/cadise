#include "core/surfaceInfo.h"

namespace cadise {

SurfaceInfo::SurfaceInfo() :
    SurfaceInfo(Vector3R(0.0_r), Vector3R(0.0_r)) {
}

SurfaceInfo::SurfaceInfo(const Vector3R point, const Vector3R normal) :
    _point(point), _normal(normal) {
}

Vector3R SurfaceInfo::point() const {
    return _point;
}

Vector3R SurfaceInfo::normal() const {
    return _normal;
}

void SurfaceInfo::setPoint(const Vector3R point) {
    _point = point;
}

void SurfaceInfo::setNormal(const Vector3R normal) {
    _normal = normal;
}

} // namespace cadise