#pragma once

#include "Core/Intersector/Accelerator/KDTree/TKDTreeNode.h"
#include "Math/Type/MathType.h"

#include <memory>
#include <tuple>
#include <vector>

// forward declaration
namespace cadise
{
    class Intersector;
}

namespace cadise 
{

class KDTreeBuilder 
{
private:
    using KDTreeNode = TKDTreeNode<std::size_t>;

public:
    KDTreeBuilder(
        const real traversalCost, 
        const real intersectionCost,
        const real emptyBonus);

    void buildNodes(
        const std::vector<std::shared_ptr<Intersector>>& intersectors,
        const std::vector<AABB3R>&                       intersectorBounds,
        const AABB3R&                                    entireBound,
        std::vector<KDTreeNode>* const                   out_nodes,
        std::vector<std::size_t>* const                  out_intersectorIndices);

private:
    void _buildNodesRecursively(
        const std::vector<std::size_t>& intersectorIndices, 
        const std::vector<AABB3R>&      intersectorBounds,
        const AABB3R&                   entireBound,
        const std::size_t               currentDepth,
        const std::size_t               badRefines,
        std::vector<KDTreeNode>* const  out_nodes,
        std::vector<std::size_t>* const out_intersectorIndices) const;

    bool _canSplitWithSah(
        const std::vector<std::size_t>&      intersectorIndices,
        const std::vector<AABB3R>&           intersectorBounds,
        const AABB3R&                        entireBound,
        const std::size_t                    badRefines,
        std::size_t* const                   out_newBadRefines,
        std::tuple<std::size_t, real>* const out_splitInfo,
        std::vector<std::size_t>* const      out_subIntersectorIndicesA,
        std::vector<std::size_t>* const      out_subIntersectorIndicesB) const;

    real        _traversalCost;
    real        _intersectionCost;
    real        _emptyBonus;
    std::size_t _maxDepth;

    static const std::size_t MAX_INTERSECTOR_SIZE = 1;
};

} // namespace cadise