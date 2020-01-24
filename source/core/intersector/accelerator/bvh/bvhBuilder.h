#pragma once

#include "core/intersector/accelerator/bvh/bvhSplitMode.h"
#include "core/intersector/accelerator/bvh/bvhLinearNode.h"

#include <memory>
#include <vector>

namespace cadise {

class BvhBinaryNode;
class BvhBoundInfo;
class Intersector;

class BvhBuilder {
public:
    BvhBuilder(const BvhSplitMode& splitMode);

    std::unique_ptr<BvhBinaryNode> buildBinaryNodes(
        const std::vector<std::shared_ptr<Intersector>>& intersectors, 
        std::vector<std::shared_ptr<Intersector>>* const out_orderedIntersectors,
        std::size_t* const                               out_totalNodeSize) const;

    void buildLinearNodes(std::unique_ptr<BvhBinaryNode>    root, 
                          const std::size_t                 totalNodeSize,
                          std::vector<BvhLinearNode>* const out_linearNodes) const;

private:
    std::unique_ptr<BvhBinaryNode> _buildBinaryNodesRecursively(
        const std::vector<BvhBoundInfo>&                  boundInfos,
        const std::vector<std::shared_ptr<Intersector>>&  intersectors,
        std::vector<std::shared_ptr<Intersector>>* const  out_orderedIntersectors,
        std::size_t* const                                out_totalNodeSize) const;

    void _buildLinearNodesRecursively(
        std::unique_ptr<BvhBinaryNode>    binaryNode, 
        std::vector<BvhLinearNode>* const out_linearNodes,
        std::size_t* const                out_nodeIndex) const;

    bool _splitWithEqual(
        const std::vector<BvhBoundInfo>& boundInfos,
        const std::size_t                splitAxis,
        std::vector<BvhBoundInfo>* const out_subBoundInfosA,
        std::vector<BvhBoundInfo>* const out_subBoundInfosB) const;

    bool _splitWithSah(
        const std::vector<BvhBoundInfo>& boundInfos,
        const std::size_t                splitAxis,
        const AABB3R&                    intersectorBound,
        const AABB3R&                    centroidBound,
        std::vector<BvhBoundInfo>* const out_subBoundInfosA,
        std::vector<BvhBoundInfo>* const out_subBoundInfosB) const;

    BvhSplitMode _splitMode;

    static const std::size_t MAX_INTERSECTOR_SIZE = 1;
};

} // namespace cadise