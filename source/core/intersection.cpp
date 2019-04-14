#include "core/intersection.h"

namespace cadise {

Intersection::Intersection() {
}

void Intersection::setSurfaceInfo(SurfaceInfo surfaceInfo) {
    _surfaceInfo = surfaceInfo;
}

void Intersection::setPrimitive(Primitive primitive) {
    _primitive = primitive;
}

SurfaceInfo& Intersection::surfaceInfo() {
    return _surfaceInfo;
}

Primitive Intersection::primitive() {
    return _primitive;
}

} // namespace cadise