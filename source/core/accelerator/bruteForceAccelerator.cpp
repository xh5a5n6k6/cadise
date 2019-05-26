#include "core/accelerator/bruteForceAccelerator.h"

namespace cadise {

BruteForceAccelerator::BruteForceAccelerator(std::vector<std::shared_ptr<Intersector> > intersectors) :
    _intersectors(std::move(intersectors)) {
}

bool BruteForceAccelerator::isIntersecting(Ray &ray, Intersection &intersection) {
    bool result = false;
    for (int i = 0; i < _intersectors.size(); i++) {
        result |= _intersectors[i]->isIntersecting(ray, intersection);
    }

    return result;
}

bool BruteForceAccelerator::isOccluded(Ray &ray) {
    bool result = false;
    for (int i = 0; i < _intersectors.size(); i++) {
        result |= _intersectors[i]->isOccluded(ray);
    }

    return result;
}

} // namespace cadise