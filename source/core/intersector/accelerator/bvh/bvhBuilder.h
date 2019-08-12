#pragma once

#include "core/intersector/accelerator/bvh/bvhSplitter.h"
#include "core/intersector/accelerator/bvh/bvhLinearNode.h"

#include <memory>
#include <vector>

namespace cadise {

class BvhBinaryNode;
class Intersector;

class BvhBuilder {
public:
    BvhBuilder(const BvhSplitter& splitter);

    std::unique_ptr<BvhBinaryNode> buildBinaryNodes(
        const std::vector<std::shared_ptr<Intersector>>& intersectors, 
        std::vector<std::shared_ptr<Intersector>>& orderedIntersectors,
        uint64& totalSize) const;

    void buildLinearNodes(std::unique_ptr<BvhBinaryNode> root, 
                          std::vector<BvhLinearNode>& linearNodes,
                          const uint64 totalSize) const;

private:
    std::unique_ptr<BvhBinaryNode> _buildBinaryNodesRecursively(
        const std::vector<std::shared_ptr<Intersector>>& intersectors,
        std::vector<std::shared_ptr<Intersector>>& orderedIntersectors,
        const uint64 startIndex,
        uint64& totalSize) const;

    void _buildLinearNodesRecursively(std::unique_ptr<BvhBinaryNode> binaryNode, 
                                      std::vector<BvhLinearNode>& linearNodes,
                                      std::shared_ptr<uint64> nodeIndex) const;

    bool _splitWith_EQUAL(const std::vector<std::shared_ptr<Intersector>>& intersectors,
                          const uint32 splitAxis,
                          std::vector<std::shared_ptr<Intersector>>& subIntersectors1,
                          std::vector<std::shared_ptr<Intersector>>& subIntersectors2) const;

    BvhSplitter _splitter;

    static const uint64 MAX_INTERSECTOR_SIZE = 1;
};

} // namespace cadise