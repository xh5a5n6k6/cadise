#include "core/surfaceInfo.h"

namespace cadise {

SurfaceInfo::SurfaceInfo() :
    SurfaceInfo(Vector3F(0.0f), Vector3F(0.0f)) {
}

SurfaceInfo::SurfaceInfo(Vector3F point, Vector3F normal) :
    _point(point), _normal(normal) {
}

Vector3F SurfaceInfo::point() {
    return _point;
}

Vector3F SurfaceInfo::normal() {
    return _normal;
}

void SurfaceInfo::setPoint(Vector3F point) {
    _point = point;
}

void SurfaceInfo::setNormal(Vector3F normal) {
    _normal = normal;
}

} // namespace cadise