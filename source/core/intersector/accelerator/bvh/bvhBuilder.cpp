#include "core/intersector/accelerator/bvh/bvhBuilder.h"

#include "core/intersector/accelerator/bvh/bvhBinaryNode.h"
#include "core/intersector/intersector.h"
#include "fundamental/assertion.h"

#include <algorithm>

namespace cadise {

BvhBuilder::BvhBuilder(const BvhSplitMode& splitMode) :
    _splitMode(splitMode) {
}

std::unique_ptr<BvhBinaryNode> BvhBuilder::buildBinaryNodes(
    const std::vector<std::shared_ptr<Intersector>>& intersectors, 
    std::vector<std::shared_ptr<Intersector>>* const out_orderedIntersectors,
    std::size_t* const                               out_totalSize) const {

    CADISE_ASSERT(out_totalSize);

    std::unique_ptr<BvhBinaryNode> root = nullptr;
    root = _buildBinaryNodesRecursively(0, 
                                        intersectors, 
                                        out_orderedIntersectors, 
                                        out_totalSize);

    return std::move(root);
}

void BvhBuilder::buildLinearNodes(std::unique_ptr<BvhBinaryNode>    root, 
                                  const std::size_t                 totalSize,
                                  std::vector<BvhLinearNode>* const out_linearNodes) const {
    
    CADISE_ASSERT(out_linearNodes);
    
    std::vector<BvhLinearNode> nodes;
    nodes.reserve(totalSize);

    _buildLinearNodesRecursively(std::move(root), &nodes, nullptr);

    out_linearNodes->swap(nodes);
}

std::unique_ptr<BvhBinaryNode> BvhBuilder::_buildBinaryNodesRecursively(
    const std::size_t                                startIndex,
    const std::vector<std::shared_ptr<Intersector>>& intersectors, 
    std::vector<std::shared_ptr<Intersector>>* const out_orderedIntersectors,
    std::size_t* const                               out_totalSize) const {

    CADISE_ASSERT(out_orderedIntersectors);
    CADISE_ASSERT(out_totalSize);

    std::unique_ptr<BvhBinaryNode> node = std::make_unique<BvhBinaryNode>();

    if (intersectors.size() <= MAX_INTERSECTOR_SIZE) {
        AABB3R leafNodeBound;
        for (std::size_t i = 0; i < intersectors.size(); ++i) {
            out_orderedIntersectors->push_back(intersectors[i]);
            leafNodeBound.unionWith(intersectors[i]->bound());
        }

        node->initializeLeafNode(leafNodeBound, startIndex, intersectors.size());
    }
    else {
        AABB3R internalNodeBound;
        for (std::size_t i = 0; i < intersectors.size(); ++i) {
            internalNodeBound.unionWith(intersectors[i]->bound());
        }
        const std::size_t splitAxis = internalNodeBound.maxAxis();
        std::vector<std::shared_ptr<Intersector>> subIntersectorsA;
        std::vector<std::shared_ptr<Intersector>> subIntersectorsB;

        switch (_splitMode) {
            case BvhSplitMode::EQUAL:
                _splitWith_EQUAL(intersectors,
                                 splitAxis,
                                 &subIntersectorsA, 
                                 &subIntersectorsB);
                break;

            default:
                break;
        }

        std::unique_ptr<BvhBinaryNode> firstChild = nullptr;
        firstChild = _buildBinaryNodesRecursively(startIndex, 
                                                  subIntersectorsA,
                                                  out_orderedIntersectors,
                                                  out_totalSize);

        std::unique_ptr<BvhBinaryNode> secondChild = nullptr;
        secondChild = _buildBinaryNodesRecursively(startIndex + subIntersectorsA.size(), 
                                                   subIntersectorsB,
                                                   out_orderedIntersectors,
                                                   out_totalSize);

        node->initializeInternalNode(std::move(firstChild),
                                     std::move(secondChild),
                                     splitAxis);
    }

    *out_totalSize += 1;
    return std::move(node);
}

void BvhBuilder::_buildLinearNodesRecursively(
    std::unique_ptr<BvhBinaryNode>    binaryNode, 
    std::vector<BvhLinearNode>* const out_linearNodes,
    std::size_t* const                out_nodeIndex) const {
    
    CADISE_ASSERT(out_linearNodes);
    
    BvhLinearNode linearNode;
    const std::size_t index = out_linearNodes->size();
    if (out_nodeIndex) {
        *out_nodeIndex = index;
    }

    if (binaryNode->isLeaf()) {
        linearNode.initializeLeafNode(binaryNode->bound(), 
                                      binaryNode->intersectorIndex(), 
                                      binaryNode->intersectorCounts());
        out_linearNodes->push_back(linearNode);
    }
    else {
        out_linearNodes->push_back(linearNode);
        
        const AABB3R internalNodeBound = binaryNode->bound();
        const std::size_t internalNodeSplitAxis = binaryNode->splitAxis();
        std::size_t secondChildIndex;

        _buildLinearNodesRecursively(std::move(binaryNode->firstChild()), 
                                     out_linearNodes, 
                                     nullptr);

        _buildLinearNodesRecursively(std::move(binaryNode->secondChild()), 
                                     out_linearNodes, 
                                     &secondChildIndex);

        (*out_linearNodes)[index].initializeInternalNode(internalNodeBound, 
                                                         secondChildIndex, 
                                                         internalNodeSplitAxis);
    }
}

bool BvhBuilder::_splitWith_EQUAL(
    const std::vector<std::shared_ptr<Intersector>>& intersectors,
    const std::size_t                                splitAxis,
    std::vector<std::shared_ptr<Intersector>>* const out_subIntersectorsA,
    std::vector<std::shared_ptr<Intersector>>* const out_subIntersectorsB) const {
    
    CADISE_ASSERT(out_subIntersectorsA);
    CADISE_ASSERT(out_subIntersectorsB);

    const std::size_t size = (intersectors.size() + 1) / 2;
    out_subIntersectorsA->reserve(size);
    out_subIntersectorsB->reserve(size);

    std::vector<std::shared_ptr<Intersector>> sortedIntersectors(intersectors);
    std::nth_element(sortedIntersectors.begin(),
                     sortedIntersectors.begin() + size,
                     sortedIntersectors.end(),
                     [splitAxis](const std::shared_ptr<Intersector>& iA, const std::shared_ptr<Intersector>& iB) {
                         return iA->bound().centroid()[splitAxis] < iB->bound().centroid()[splitAxis];
                     });

    out_subIntersectorsA->insert(out_subIntersectorsA->end(),
                                 sortedIntersectors.begin(),
                                 sortedIntersectors.begin() + size);

    out_subIntersectorsB->insert(out_subIntersectorsB->end(),
                                 sortedIntersectors.begin() + size,
                                 sortedIntersectors.end());
    return true;
}

} // namespace cadise