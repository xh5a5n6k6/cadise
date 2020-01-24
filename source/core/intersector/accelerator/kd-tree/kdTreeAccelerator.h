#pragma once

#include "core/intersector/accelerator/accelerator.h"

#include "core/intersector/accelerator/kd-tree/kdTreeNode.h"
#include "math/aabb.h"

#include <memory>
#include <vector>

namespace cadise {

class KdTreeAccelerator : public Accelerator {
public:
    KdTreeAccelerator(
        const std::vector<std::shared_ptr<Intersector>>& intersectors,
        const real                                       traversalCost,
        const real                                       intersectionCost,
        const real                                       emptyBonus);

    AABB3R bound() const override;

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