#pragma once

#include "core/intersector/accelerator/accelerator.h"

#include "core/intersector/accelerator/kd-tree/tKdTreeNode.h"
#include "math/tAabb3.h"

#include <memory>
#include <vector>

namespace cadise {

class KdTreeAccelerator : public Accelerator {
private:
    using KdTreeNode = TKdTreeNode<std::size_t>;

public:
    KdTreeAccelerator(
        const std::vector<std::shared_ptr<Intersector>>& intersectors,
        const real                                       traversalCost,
        const real                                       intersectionCost,
        const real                                       emptyBonus);

    void evaluateBound(AABB3R* const out_bound) const override;

    bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override;
    bool isOccluded(const Ray& ray) const override;

private:
    std::vector<std::shared_ptr<Intersector>> _intersectors;
    std::vector<KdTreeNode>                   _nodes;

    // it stores intersector indices all nodes point to,
    // its size may be more than total intersectors because 
    // an intersector may store in more than one node.
    std::vector<std::size_t> _intersectorIndices;

    AABB3R _bound;

    static const std::size_t MAX_STACK_SIZE = 64;
};

} // namespace cadise