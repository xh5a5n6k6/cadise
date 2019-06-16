#pragma once

#include "core/intersector/accelerator/bvh/bvhSplitter.h"
#include "core/intersector/accelerator/bvh/bvhLinearNode.h"

#include <memory>
#include <vector>

namespace cadise {

class BVHBinaryNode;
class Intersector;

class BVHBuilder {
public:
    BVHBuilder(const BVHSplitter splitter);

    std::unique_ptr<BVHBinaryNode> buildBinaryNodes(
        const std::vector<std::shared_ptr<Intersector> > intersectors, 
        std::vector<std::shared_ptr<Intersector> > &orderedIntersectors,
        uint64 &totalSize) const;

    std::vector<BVHLinearNode> buildLinearNodes(std::unique_ptr<BVHBinaryNode> root, const uint64 totalSize) const;

private:
    std::unique_ptr<BVHBinaryNode> _buildBinaryNodesRecursively(
        const std::vector<std::shared_ptr<Intersector> > intersectors,
        std::vector<std::shared_ptr<Intersector> > &orderedIntersectors,
        const uint64 startIndex,
        uint64 &totalSize) const;

    void _buildLinearNodesRecursively(std::unique_ptr<BVHBinaryNode> binaryNode, 
                                      std::vector<BVHLinearNode> &linearNodes,
                                      std::shared_ptr<uint64> nodeIndex) const;

    bool _splitWith_EQUAL(const std::vector<std::shared_ptr<Intersector> > intersectors,
                          const uint32 splitAxis,
                          std::vector<std::shared_ptr<Intersector> > &subIntersectors1,
                          std::vector<std::shared_ptr<Intersector> > &subIntersectors2) const;

    BVHSplitter _splitter;

    static const uint64 MAX_INTERSECTOR_SIZE = 1;
};

} // namespace cadise