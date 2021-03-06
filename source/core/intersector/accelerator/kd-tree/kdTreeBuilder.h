#pragma once

#include "core/intersector/accelerator/kd-tree/tKdTreeNode.h"
#include "math/type/mathType.h"

#include <memory>
#include <tuple>
#include <vector>

namespace cadise {

class Intersector;

class KdTreeBuilder {
private:
    using KdTreeNode = TKdTreeNode<std::size_t>;

public:
    KdTreeBuilder(
        const real traversalCost, 
        const real intersectionCost,
        const real emptyBonus);

    void buildNodes(
        const std::vector<std::shared_ptr<Intersector>>& intersectors,
        const std::vector<AABB3R>&                       intersectorBounds,
        const AABB3R&                                    entireBound,
        std::vector<KdTreeNode>* const                   out_nodes,
        std::vector<std::size_t>* const                  out_intersectorIndices);

private:
    void _buildNodesRecursively(
        const std::vector<std::size_t>& intersectorIndices, 
        const std::vector<AABB3R>&      intersectorBounds,
        const AABB3R&                   entireBound,
        const std::size_t               currentDepth,
        const std::size_t               badRefines,
        std::vector<KdTreeNode>* const  out_nodes,
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