#pragma once

#include "core/intersector/accelerator/kd-tree/tKdTreeNode.h"
#include "math/tAabb3.h"

#include <tuple>
#include <vector>

namespace cadise
{

template<typename Index, typename Object, typename ObjectCenterCalculator>
class TPointKdTree
{
private:
    using Node = TKdTreeNode<Index>;

public:
    explicit TPointKdTree(const ObjectCenterCalculator& centerCalculator);

    void buildNodes(const std::vector<Object>& objects);

    void findWithRange(
        const Vector3R&            position,
        const real                 searchRadius,
        std::vector<Object>* const out_objects) const;

private:
    void _buildNodesRecursively(
        const std::vector<Vector3R>& objectCenters,
        const std::vector<Index>&    objectIndices,
        const AABB3R&                entireBound,
        const std::size_t            currentDepth);

    bool _canSplitWithEqual(
        const std::vector<Vector3R>&         objectCenters,
        const std::vector<Index>&            objectIndices,
        const AABB3R&                        entireBound,
        std::tuple<std::size_t, real>* const out_splitInfo,
        std::vector<Index>* const            out_subObjectIndicesA,
        std::vector<Index>* const            out_subObjectIndicesB);

    std::vector<Object> _objects;
    std::vector<Node>   _nodes;
    std::vector<Index>  _objectIndices;

    AABB3R                 _bound;
    ObjectCenterCalculator _centerCalculator;

    const static std::size_t MAX_OBJECT_SIZE = 2;
    const static std::size_t MAX_STACK_SIZE  = 64;
};

} // namespace cadise

#include "core/integral-tool/tPointKdTree.ipp"