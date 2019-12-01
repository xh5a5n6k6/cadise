#pragma once

#include "core/intersector/accelerator/bvh/bvhSplitMode.h"
#include "core/intersector/accelerator/bvh/bvhLinearNode.h"

#include <memory>
#include <vector>

namespace cadise {

class BvhBinaryNode;
class Intersector;

class BvhBuilder {
public:
    BvhBuilder(const BvhSplitMode& splitMode);

    std::unique_ptr<BvhBinaryNode> buildBinaryNodes(
        const std::vector<std::shared_ptr<Intersector>>& intersectors, 
        std::vector<std::shared_ptr<Intersector>>* const out_orderedIntersectors,
        std::size_t* const out_totalSize) const;

    void buildLinearNodes(std::unique_ptr<BvhBinaryNode> root, 
                          std::vector<BvhLinearNode>* const out_linearNodes,
                          const std::size_t totalSize) const;

private:
    std::unique_ptr<BvhBinaryNode> _buildBinaryNodesRecursively(
        const std::vector<std::shared_ptr<Intersector>>& intersectors,
        std::vector<std::shared_ptr<Intersector>>* const out_orderedIntersectors,
        const std::size_t startIndex,
        std::size_t* const out_totalSize) const;

    void _buildLinearNodesRecursively(std::unique_ptr<BvhBinaryNode> binaryNode, 
                                      std::vector<BvhLinearNode>* const out_linearNodes,
                                      std::size_t* const out_nodeIndex) const;

    bool _splitWith_EQUAL(const std::vector<std::shared_ptr<Intersector>>& intersectors,
                          const std::size_t splitAxis,
                          std::vector<std::shared_ptr<Intersector>>* const out_subIntersectorsA,
                          std::vector<std::shared_ptr<Intersector>>* const out_subIntersectorsB) const;

    BvhSplitMode _splitMode;

    static const std::size_t MAX_INTERSECTOR_SIZE = 1;
};

} // namespace cadise