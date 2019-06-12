#include "core/intersection.h"

namespace cadise {

Intersection::Intersection() :
    _surfaceInfo(), _intersector(nullptr) {
}

void Intersection::setSurfaceInfo(const SurfaceInfo surfaceInfo) {
    _surfaceInfo = surfaceInfo;
}

void Intersection::setIntersector(const std::shared_ptr<Intersector> intersector) {
    _intersector = intersector;
}

SurfaceInfo Intersection::surfaceInfo() const {
    return _surfaceInfo;
}

std::shared_ptr<Intersector> Intersection::intersector() const {
    return _intersector;
}

} // namespace cadise