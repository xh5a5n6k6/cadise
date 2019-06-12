#include "core/intersector/accelerator/bruteForceAccelerator.h"

namespace cadise {

BruteForceAccelerator::BruteForceAccelerator(const std::vector<std::shared_ptr<Intersector> > intersectors) :
    _intersectors(std::move(intersectors)) {
}

bool BruteForceAccelerator::isIntersecting(Ray &ray, Intersection &intersection) const {
    bool result = false;
    for (uint64 i = 0; i < _intersectors.size(); i++) {
        result |= _intersectors[i]->isIntersecting(ray, intersection);
    }

    return result;
}

bool BruteForceAccelerator::isOccluded(Ray &ray) const {
    bool result = false;
    for (uint64 i = 0; i < _intersectors.size(); i++) {
        result |= _intersectors[i]->isOccluded(ray);
    }

    return result;
}

} // namespace cadise