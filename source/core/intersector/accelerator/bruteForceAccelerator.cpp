#include "core/intersector/accelerator/bruteForceAccelerator.h"

#include "math/aabb.h"

namespace cadise {

BruteForceAccelerator::BruteForceAccelerator(const std::vector<std::shared_ptr<Intersector>>& intersectors) :
    _intersectors(std::move(intersectors)) {
}

AABB3R BruteForceAccelerator::bound() const {
    AABB3R result;
    for (std::size_t i = 0; i < _intersectors.size(); i++) {
        result.unionWith(_intersectors[i]->bound());
    }

    return result;
}

bool BruteForceAccelerator::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const {
    bool result = false;
    for (std::size_t i = 0; i < _intersectors.size(); i++) {
        result |= _intersectors[i]->isIntersecting(ray, primitiveInfo);
    }

    return result;
}

bool BruteForceAccelerator::isOccluded(const Ray& ray) const {
    for (std::size_t i = 0; i < _intersectors.size(); i++) {
        if (_intersectors[i]->isOccluded(ray)) {
            return true;
        }
    }

    return false;
}

} // namespace cadise