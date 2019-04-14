#include "core/surfaceInfo.h"

namespace cadise {

SurfaceInfo::SurfaceInfo() :
    SurfaceInfo(Vector3(), Vector3()) {
}

SurfaceInfo::SurfaceInfo(Vector3 hitPoint, Vector3 hitNormal) :
    _hitPoint(hitPoint), _hitNormal(hitNormal) {
}

Vector3 SurfaceInfo::hitPoint() {
    return _hitPoint;
}

Vector3 SurfaceInfo::hitNormal() {
    return _hitNormal;
}

void SurfaceInfo::setHitPoint(Vector3 hitPoint) {
    _hitPoint = hitPoint;
}

void SurfaceInfo::setHitNormal(Vector3 hitNormal) {
    _hitNormal = hitNormal;
}

} // namespace cadise