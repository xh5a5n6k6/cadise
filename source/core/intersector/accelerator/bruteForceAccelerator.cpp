#include "core/intersector/accelerator/bruteForceAccelerator.h"

namespace cadise {

BruteForceAccelerator::BruteForceAccelerator(const std::vector<std::shared_ptr<Intersector>>& intersectors) :
    _intersectors(std::move(intersectors)) {
}

bool BruteForceAccelerator::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const {
    bool result = false;
    for (uint64 index = 0; index < _intersectors.size(); index++) {
        result |= _intersectors[index]->isIntersecting(ray, primitiveInfo);
    }

    return result;
}

bool BruteForceAccelerator::isOccluded(Ray& ray) const {
    for (uint64 index = 0; index < _intersectors.size(); index++) {
        if (_intersectors[index]->isOccluded(ray)) {
            return true;
        }
    }

    return false;
}

} // namespace cadise