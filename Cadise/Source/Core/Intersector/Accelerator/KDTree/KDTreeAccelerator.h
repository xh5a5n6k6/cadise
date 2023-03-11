#pragma once

#include "Core/Intersector/Accelerator/Accelerator.h"

#include "Core/Intersector/Accelerator/KDTree/TKDTreeNode.h"
#include "Math/TAABB3.h"

#include <memory>
#include <vector>

namespace cadise 
{

class KDTreeAccelerator : public Accelerator 
{
private:
    using KDTreeNode = TKDTreeNode<std::size_t>;

public:
    KDTreeAccelerator(
        const std::vector<std::shared_ptr<Intersector>>& intersectors,
        const real                                       traversalCost,
        const real                                       intersectionCost,
        const real                                       emptyBonus);

    void evaluateBound(AABB3R* const out_bound) const override;

    bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override;
    bool isOccluded(const Ray& ray) const override;

private:
    std::vector<std::shared_ptr<Intersector>> _intersectors;
    std::vector<KDTreeNode>                   _nodes;

    // it stores intersector indices all nodes point to,
    // its size may be more than total intersectors because 
    // an intersector may store in more than one node.
    std::vector<std::size_t> _intersectorIndices;

    AABB3R _bound;

    static const std::size_t MAX_STACK_SIZE = 64;
};

} // namespace cadise