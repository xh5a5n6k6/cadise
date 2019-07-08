#include "core/surfaceGeometryInfo.h"

namespace cadise {

SurfaceGeometryInfo::SurfaceGeometryInfo() :
    SurfaceGeometryInfo(Vector3R(0.0_r), Vector3R(0.0_r)) {
}

SurfaceGeometryInfo::SurfaceGeometryInfo(const Vector3R point, const Vector3R normal) :
    _point(point), _normal(normal) {
}

Vector3R SurfaceGeometryInfo::point() const {
    return _point;
}

Vector3R SurfaceGeometryInfo::normal() const {
    return _normal;
}

void SurfaceGeometryInfo::setPoint(const Vector3R point) {
    _point = point;
}

void SurfaceGeometryInfo::setNormal(const Vector3R normal) {
    _normal = normal;
}

} // namespace cadise