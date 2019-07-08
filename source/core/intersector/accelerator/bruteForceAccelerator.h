#pragma once

#include "core/intersector/accelerator/accelerator.h"

#include <memory>
#include <vector>

namespace cadise {

// BruteForceAccelerator is for no acceleration. 
// It is for debugging usage compared to other accelerators.
class BruteForceAccelerator : public Accelerator {
public:
    BruteForceAccelerator(const std::vector<std::shared_ptr<Intersector>>& intersectors);

    bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override;
    bool isOccluded(Ray& ray) const override;

private:
    std::vector<std::shared_ptr<Intersector>> _intersectors;
};

} // namespace cadise