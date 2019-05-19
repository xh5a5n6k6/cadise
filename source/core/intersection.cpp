#include "core/intersection.h"

namespace cadise {

Intersection::Intersection() :
    _surfaceInfo(), _intersector(nullptr) {
}

void Intersection::setSurfaceInfo(SurfaceInfo surfaceInfo) {
    _surfaceInfo = surfaceInfo;
}

void Intersection::setIntersector(std::shared_ptr<Intersector> intersector) {
    _intersector = intersector;
}

SurfaceInfo& Intersection::surfaceInfo() {
    return _surfaceInfo;
}

std::shared_ptr<Intersector> Intersection::intersector() {
    return _intersector;
}

} // namespace cadise