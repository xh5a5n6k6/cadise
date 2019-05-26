#pragma once

#include "core/accelerator/accelerator.h"

#include <memory>
#include <vector>

namespace cadise {

// BruteForceAccelerator is for no acceleration. 
// It is for debugging usage compared to other accelerators.
class BruteForceAccelerator : public Accelerator {
public:
    BruteForceAccelerator(std::vector<std::shared_ptr<Intersector> > intersectors);

    bool isIntersecting(Ray &ray, Intersection &intersection) override;
    bool isOccluded(Ray &ray) override;

private:
    std::vector<std::shared_ptr<Intersector> > _intersectors;
};

} // namespace cadise