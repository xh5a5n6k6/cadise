#pragma once

#include "core/accelerator/accelerator.h"

#include <memory>
#include <vector>

namespace cadise {

class BruteAccelerator : public Accelerator {
public:
    BruteAccelerator(std::vector<std::shared_ptr<Intersector> > intersectors);

    bool isIntersecting(Ray &ray, Intersection &intersection) override;
    bool isOccluded(Ray &ray) override;

private:
    std::vector<std::shared_ptr<Intersector> > _intersectors;
};

} // namespace cadise