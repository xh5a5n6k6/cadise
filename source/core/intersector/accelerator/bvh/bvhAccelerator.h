#pragma once

#include "core/intersector/accelerator/accelerator.h"

#include "core/intersector/accelerator/bvh/bvhLinearNode.h"

#include <memory>
#include <vector>

namespace cadise {

class BVHAccelerator : public Accelerator {
public:
    BVHAccelerator(const std::vector<std::shared_ptr<Intersector> > intersectors);

    AABB3R bound() const override;

    bool isIntersecting(Ray &ray, Intersection &intersection) const override;
    bool isOccluded(Ray &ray) const override;

private:
    std::vector<std::shared_ptr<Intersector> > _intersectors;
    std::vector<BVHLinearNode> _nodes;

    static const uint64 MAX_STACK_SIZE = 64;
};

} // namespace cadise