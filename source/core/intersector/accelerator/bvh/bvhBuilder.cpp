#include "core/intersector/accelerator/bvh/bvhBuilder.h"

#include "core/intersector/accelerator/bvh/bvhBinaryNode.h"
#include "core/intersector/intersector.h"

#include <algorithm>

namespace cadise {

BvhBuilder::BvhBuilder(const BvhSplitter& splitter) :
    _splitter(splitter) {
}

std::unique_ptr<BvhBinaryNode> BvhBuilder::buildBinaryNodes(
    const std::vector<std::shared_ptr<Intersector>>& intersectors, 
    std::vector<std::shared_ptr<Intersector>>& orderedIntersectors,
    std::size_t& totalSize) const {

    std::unique_ptr<BvhBinaryNode> root = nullptr;
    root = _buildBinaryNodesRecursively(intersectors, orderedIntersectors, 0, totalSize);

    return std::move(root);
}

void BvhBuilder::buildLinearNodes(std::unique_ptr<BvhBinaryNode> root, 
                                  std::vector<BvhLinearNode>& linearNodes,
                                  const std::size_t totalSize) const {
    std::vector<BvhLinearNode> nodes;
    nodes.reserve(totalSize);

    _buildLinearNodesRecursively(std::move(root), nodes, nullptr);

    linearNodes.swap(nodes);
}

std::unique_ptr<BvhBinaryNode> BvhBuilder::_buildBinaryNodesRecursively(
    const std::vector<std::shared_ptr<Intersector>>& intersectors, 
    std::vector<std::shared_ptr<Intersector>>& orderedIntersectors,
    const std::size_t startIndex,
    std::size_t& totalSize) const {

    std::unique_ptr<BvhBinaryNode> node = std::make_unique<BvhBinaryNode>();

    if (intersectors.size() <= MAX_INTERSECTOR_SIZE) {
        AABB3R leafNodeBound;
        for (std::size_t i = 0; i < intersectors.size(); ++i) {
            orderedIntersectors.push_back(intersectors[i]);
            leafNodeBound.unionWith(intersectors[i]->bound());
        }

        node->initializeLeafNode(leafNodeBound, startIndex, intersectors.size());
    }
    else {
        AABB3R internalNodeBound;
        for (std::size_t i = 0; i < intersectors.size(); ++i) {
            internalNodeBound.unionWith(intersectors[i]->bound());
        }
        std::size_t splitAxis = internalNodeBound.maxAxis();
        std::vector<std::shared_ptr<Intersector>> subIntersectorsA;
        std::vector<std::shared_ptr<Intersector>> subIntersectorsB;

        switch (_splitter) {
            case BvhSplitter::EQUAL:
                _splitWith_EQUAL(intersectors,
                                 splitAxis,
                                 subIntersectorsA, 
                                 subIntersectorsB);
                break;

            default:
                break;
        }

        std::unique_ptr<BvhBinaryNode> firstChild = nullptr;
        firstChild = _buildBinaryNodesRecursively(subIntersectorsA,
                                                  orderedIntersectors,
                                                  startIndex,
                                                  totalSize);

        std::unique_ptr<BvhBinaryNode> secondChild = nullptr;
        secondChild = _buildBinaryNodesRecursively(subIntersectorsB,
                                                   orderedIntersectors,
                                                   startIndex + subIntersectorsA.size(),
                                                   totalSize);

        node->initializeInternalNode(std::move(firstChild),
                                     std::move(secondChild),
                                     splitAxis);
    }

    totalSize += 1;
    return std::move(node);
}

void BvhBuilder::_buildLinearNodesRecursively(std::unique_ptr<BvhBinaryNode> binaryNode, 
                                              std::vector<BvhLinearNode>& linearNodes,
                                              std::size_t* const nodeIndex) const {
    BvhLinearNode linearNode;
    std::size_t index = linearNodes.size();
    if (nodeIndex) {
        *nodeIndex = index;
    }

    if (binaryNode->isLeaf()) {
        linearNode.initializeLeafNode(binaryNode->bound(), 
                                      binaryNode->intersectorIndex(), 
                                      binaryNode->intersectorCounts());
        linearNodes.push_back(linearNode);
    }
    else {
        linearNodes.push_back(linearNode);
        AABB3R internalNodeBound = binaryNode->bound();
        std::size_t internalNodeSplitAxis = binaryNode->splitAxis();
        std::size_t secondChildIndex;

        _buildLinearNodesRecursively(std::move(binaryNode->firstChild()), linearNodes, nullptr);
        _buildLinearNodesRecursively(std::move(binaryNode->secondChild()), linearNodes, &secondChildIndex);

        linearNodes[index].initializeInternalNode(internalNodeBound, secondChildIndex, internalNodeSplitAxis);
    }
}

bool BvhBuilder::_splitWith_EQUAL(const std::vector<std::shared_ptr<Intersector>>& intersectors,
                                  const std::size_t splitAxis,
                                  std::vector<std::shared_ptr<Intersector>>& subIntersectorsA,
                                  std::vector<std::shared_ptr<Intersector>>& subIntersectorsB) const {
    std::size_t size = (intersectors.size() + 1) / 2;
    subIntersectorsA.reserve(size);
    subIntersectorsB.reserve(size);

    std::vector<std::shared_ptr<Intersector>> sortedIntersectors(intersectors);
    std::nth_element(sortedIntersectors.begin(),
                     sortedIntersectors.begin() + size,
                     sortedIntersectors.end(),
                     [splitAxis](std::shared_ptr<Intersector> iA, std::shared_ptr<Intersector> iB) {
                         return iA->bound().centroid()[splitAxis] < iB->bound().centroid()[splitAxis];
                     });

    subIntersectorsA.insert(subIntersectorsA.end(),
                            sortedIntersectors.begin(),
                            sortedIntersectors.begin() + size);

    subIntersectorsB.insert(subIntersectorsB.end(),
                            sortedIntersectors.begin() + size,
                            sortedIntersectors.end());
    return true;
}

} // namespace cadise