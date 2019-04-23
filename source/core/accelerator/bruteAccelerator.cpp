#include "core/accelerator/bruteAccelerator.h"

namespace cadise {

BruteAccelerator::BruteAccelerator(std::vector<std::shared_ptr<Intersector> > intersectors) :
    _intersectors(intersectors) {
}

bool BruteAccelerator::isIntersecting(Ray &ray, Intersection &intersection) {
    bool result = false;
    for (int i = 0; i < _intersectors.size(); i++)
        result |= _intersectors[i]->isIntersecting(ray, intersection);

    return result;
}

bool BruteAccelerator::isOccluded(Ray &ray) {
    bool result = false;
    for (int i = 0; i < _intersectors.size(); i++)
        result |= _intersectors[i]->isOccluded(ray);

    return result;
}

} // namespace cadise