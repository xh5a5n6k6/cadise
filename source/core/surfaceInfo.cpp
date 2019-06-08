#include "core/surfaceInfo.h"

namespace cadise {

SurfaceInfo::SurfaceInfo() :
    SurfaceInfo(Vector3R(0.0_r), Vector3R(0.0_r)) {
}

SurfaceInfo::SurfaceInfo(Vector3R point, Vector3R normal) :
    _point(point), _normal(normal) {
}

Vector3R SurfaceInfo::point() {
    return _point;
}

Vector3R SurfaceInfo::normal() {
    return _normal;
}

void SurfaceInfo::setPoint(Vector3R point) {
    _point = point;
}

void SurfaceInfo::setNormal(Vector3R normal) {
    _normal = normal;
}

} // namespace cadise