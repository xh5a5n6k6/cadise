#pragma once

#include "Core/Intersector/Accelerator/Accelerator.h"

#include <memory>
#include <vector>

namespace cadise
{

// BruteForceAccelerator is for no acceleration. 
// It is for debugging usage compared to other accelerators.
class BruteForceAccelerator : public Accelerator
{
public:
    explicit BruteForceAccelerator(const std::vector<std::shared_ptr<Intersector>>& intersectors);

    void evaluateBound(AABB3R* const out_bound) const override;

    bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override;
    bool isOccluded(const Ray& ray) const override;

private:
    std::vector<std::shared_ptr<Intersector>> _intersectors;
};

} // namespace cadise