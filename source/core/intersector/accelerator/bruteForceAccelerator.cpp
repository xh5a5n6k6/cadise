#include "core/intersector/accelerator/bruteForceAccelerator.h"

namespace cadise {

BruteForceAccelerator::BruteForceAccelerator(const std::vector<std::shared_ptr<Intersector> > intersectors) :
    _intersectors(std::move(intersectors)) {
}

bool BruteForceAccelerator::isIntersecting(Ray &ray, Intersection &intersection) const {
    bool result = false;
    for (uint64 index = 0; index < _intersectors.size(); index++) {
        result |= _intersectors[index]->isIntersecting(ray, intersection);
    }

    return result;
}

bool BruteForceAccelerator::isOccluded(Ray &ray) const {
    bool result = false;
    for (uint64 index = 0; index < _intersectors.size(); index++) {
        result |= _intersectors[index]->isOccluded(ray);
    }

    return result;
}

} // namespace cadise