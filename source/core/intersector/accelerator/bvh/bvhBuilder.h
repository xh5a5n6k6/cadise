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
        std::size_t* const out_totalSize) const;

    void buildLinearNodes(std::unique_ptr<BvhBinaryNode> root, 
                          std::vector<BvhLinearNode>& linearNodes,
                          const std::size_t totalSize) const;

private:
    std::unique_ptr<BvhBinaryNode> _buildBinaryNodesRecursively(
        const std::vector<std::shared_ptr<Intersector>>& intersectors,
        std::vector<std::shared_ptr<Intersector>>& orderedIntersectors,
        const std::size_t startIndex,
        std::size_t* out_totalSize) const;

    void _buildLinearNodesRecursively(std::unique_ptr<BvhBinaryNode> binaryNode, 
                                      std::vector<BvhLinearNode>& linearNodes,
                                      std::size_t* const out_nodeIndex) const;

    bool _splitWith_EQUAL(const std::vector<std::shared_ptr<Intersector>>& intersectors,
                          const std::size_t splitAxis,
                          std::vector<std::shared_ptr<Intersector>>* const out_subIntersectorsA,
                          std::vector<std::shared_ptr<Intersector>>* const out_subIntersectorsB) const;

    BvhSplitter _splitter;

    static const std::size_t MAX_INTERSECTOR_SIZE = 1;
};

} // namespace cadise