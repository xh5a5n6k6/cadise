#pragma once

#include "Core/Intersector/Accelerator/BVH/BVHLinearNode.h"
#include "Core/Intersector/Accelerator/BVH/EBVHSplitMode.h"

#include <memory>
#include <vector>

// forward declaration
namespace cadise 
{ 
    class BVHBinaryNode;
    class BVHBoundInfo;
    class Intersector; 
}

namespace cadise 
{

class BVHBuilder 
{
public:
    explicit BVHBuilder(const EBVHSplitMode splitMode);

    std::unique_ptr<BVHBinaryNode> buildBinaryNodes(
        const std::vector<std::shared_ptr<Intersector>>& intersectors, 
        std::vector<std::shared_ptr<Intersector>>* const out_orderedIntersectors,
        std::size_t* const                               out_totalNodeSize) const;

    void buildLinearNodes(
        std::unique_ptr<BVHBinaryNode>    root, 
        const std::size_t                 totalNodeSize,
        std::vector<BVHLinearNode>* const out_linearNodes) const;

private:
    std::unique_ptr<BVHBinaryNode> _buildBinaryNodesRecursively(
        const std::vector<BVHBoundInfo>&                 boundInfos,
        const std::vector<std::shared_ptr<Intersector>>& intersectors,
        std::vector<std::shared_ptr<Intersector>>* const out_orderedIntersectors,
        std::size_t* const                               out_totalNodeSize) const;

    void _buildLinearNodesRecursively(
        std::unique_ptr<BVHBinaryNode>    binaryNode, 
        std::vector<BVHLinearNode>* const out_linearNodes,
        std::size_t* const                out_nodeIndex) const;

    bool _canSplitWithEqualCounts(
        const std::vector<BVHBoundInfo>& boundInfos,
        const std::size_t                splitAxis,
        std::vector<BVHBoundInfo>* const out_subBoundInfosA,
        std::vector<BVHBoundInfo>* const out_subBoundInfosB) const;

    bool _canSplitWithSah(
        const std::vector<BVHBoundInfo>& boundInfos,
        const std::size_t                splitAxis,
        const AABB3R&                    intersectorBound,
        const AABB3R&                    centroidBound,
        std::vector<BVHBoundInfo>* const out_subBoundInfosA,
        std::vector<BVHBoundInfo>* const out_subBoundInfosB) const;

    EBVHSplitMode _splitMode;

    static const std::size_t MAX_INTERSECTOR_SIZE = 1;
};

} // namespace cadise