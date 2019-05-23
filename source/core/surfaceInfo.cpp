#include "core/surfaceInfo.h"

namespace cadise {

SurfaceInfo::SurfaceInfo() :
    SurfaceInfo(Vector3(), Vector3()) {
}

SurfaceInfo::SurfaceInfo(Vector3 point, Vector3 normal) :
    _point(point), _normal(normal) {
}

Vector3 SurfaceInfo::point() {
    return _point;
}

Vector3 SurfaceInfo::normal() {
    return _normal;
}

void SurfaceInfo::setPoint(Vector3 point) {
    _point = point;
}

void SurfaceInfo::setNormal(Vector3 normal) {
    _normal = normal;
}

} // namespace cadise