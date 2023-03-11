#pragma once

#include "Core/Intersector/Accelerator/Accelerator.h"

#include "Core/Intersector/Accelerator/BVH/BVHLinearNode.h"

#include <memory>
#include <vector>

// forward declration
namespace cadise 
{ 
    enum class EBVHSplitMode; 
}

namespace cadise 
{

class BvhAccelerator : public Accelerator 
{
public:
    BvhAccelerator(
        const std::vector<std::shared_ptr<Intersector>>& intersectors,
        const EBVHSplitMode                              splitMode);

    void evaluateBound(AABB3R* const out_bound) const override;

    bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override;
    bool isOccluded(const Ray& ray) const override;

private:
    std::vector<std::shared_ptr<Intersector>> _intersectors;
    std::vector<BVHLinearNode>                _nodes;

    static const std::size_t MAX_STACK_SIZE = 64;
};

} // namespace cadise