#pragma once

#include "core/intersector/accelerator/accelerator.h"

#include "core/intersector/accelerator/bvh/bvhLinearNode.h"

#include <memory>
#include <vector>

namespace cadise {

enum class EBvhSplitMode;

class BvhAccelerator : public Accelerator {
public:
    BvhAccelerator(
        const std::vector<std::shared_ptr<Intersector>>& intersectors,
        const EBvhSplitMode&                             splitMode);

    void evaluateBound(AABB3R* const out_bound) const override;

    bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override;
    bool isOccluded(const Ray& ray) const override;

private:
    std::vector<std::shared_ptr<Intersector>> _intersectors;
    std::vector<BvhLinearNode>                _nodes;

    static const std::size_t MAX_STACK_SIZE = 64;
};

} // namespace cadise