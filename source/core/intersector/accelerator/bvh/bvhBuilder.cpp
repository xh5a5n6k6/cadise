#include "core/intersector/accelerator/bvh/bvhBuilder.h"

#include "core/intersector/accelerator/bvh/bvhBinaryNode.h"
#include "core/intersector/intersector.h"

#include <algorithm>

namespace cadise {

BVHBuilder::BVHBuilder(const BVHSplitter splitter) :
    _splitter(splitter) {
}

std::unique_ptr<BVHBinaryNode> BVHBuilder::buildBinaryNodes(
    const std::vector<std::shared_ptr<Intersector> > intersectors, 
    std::vector<std::shared_ptr<Intersector> > &orderedIntersectors,
    uint64 &totalSize) const {

    std::unique_ptr<BVHBinaryNode> root = _buildBinaryNodesRecursively(intersectors, orderedIntersectors, 0, totalSize);

    return std::move(root);
}

std::vector<BVHLinearNode> BVHBuilder::buildLinearNodes(std::unique_ptr<BVHBinaryNode> root, const uint64 totalSize) const {
    std::vector<BVHLinearNode> linearNodes;
    linearNodes.reserve(totalSize);
    
    _buildLinearNodesRecursively(std::move(root), linearNodes, nullptr);

    return linearNodes;
}

std::unique_ptr<BVHBinaryNode> BVHBuilder::_buildBinaryNodesRecursively(
    const std::vector<std::shared_ptr<Intersector> > intersectors, 
    std::vector<std::shared_ptr<Intersector> > &orderedIntersectors,
    const uint64 startIndex,
    uint64 &totalSize) const {

    std::unique_ptr<BVHBinaryNode> node = std::make_unique<BVHBinaryNode>();

    if (intersectors.size() <= MAX_INTERSECTOR_SIZE) {
        AABB3R leafNodeBound;
        for (uint64 i = 0; i < intersectors.size(); i++) {
            orderedIntersectors.push_back(intersectors[i]);
            leafNodeBound.unionWith(intersectors[i]->bound());
        }

        node->initializeLeafNode(leafNodeBound, startIndex, intersectors.size());
    }
    else {
        AABB3R internalNodeBound;
        for (uint64 i = 0; i < intersectors.size(); i++) {
            internalNodeBound.unionWith(intersectors[i]->bound());
        }
        uint32 splitAxis = internalNodeBound.maxAxis();
        std::vector<std::shared_ptr<Intersector> > subIntersectors1;
        std::vector<std::shared_ptr<Intersector> > subIntersectors2;

        switch (_splitter) {
            case BVHSplitter::EQUAL:
                _splitWith_EQUAL(intersectors,
                                 splitAxis,
                                 subIntersectors1, 
                                 subIntersectors2);
                break;

            default:
                break;
        }

        std::unique_ptr<BVHBinaryNode> firstChild = nullptr;
        firstChild = _buildBinaryNodesRecursively(subIntersectors1,
                                                  orderedIntersectors,
                                                  startIndex,
                                                  totalSize);

        std::unique_ptr<BVHBinaryNode> secondChild = nullptr;
        secondChild = _buildBinaryNodesRecursively(subIntersectors2,
                                                   orderedIntersectors,
                                                   startIndex + subIntersectors1.size(),
                                                   totalSize);

        node->initializeInternalNode(std::move(firstChild),
                                     std::move(secondChild),
                                     splitAxis);
    }

    totalSize += 1;
    return std::move(node);
}

void BVHBuilder::_buildLinearNodesRecursively(std::unique_ptr<BVHBinaryNode> binaryNode, 
                                              std::vector<BVHLinearNode> &linearNodes,
                                              std::shared_ptr<uint64> nodeIndex) const {
    BVHLinearNode linearNode;
    uint64 index = linearNodes.size();
    if (nodeIndex) {
        *nodeIndex = index;
    }

    if (binaryNode->isLeaf()) {
        linearNode.initializeLeafNode(binaryNode->bound(), binaryNode->intersectorIndex(), binaryNode->intersectorCounts());
        linearNodes.push_back(linearNode);
    }
    else {
        linearNodes.push_back(linearNode);
        AABB3R internalNodeBound = binaryNode->bound();
        uint32 internalNodeSplitAxis = binaryNode->splitAxis();
        std::shared_ptr<uint64> secondChildIndex = std::make_shared<uint64>(0);

        _buildLinearNodesRecursively(std::move(binaryNode->firstChild()), linearNodes, nullptr);
        _buildLinearNodesRecursively(std::move(binaryNode->secondChild()), linearNodes, secondChildIndex);

        linearNodes[index].initializeInternalNode(internalNodeBound, *secondChildIndex, internalNodeSplitAxis);
    }
}

bool BVHBuilder::_splitWith_EQUAL(const std::vector<std::shared_ptr<Intersector> > intersectors,
                                  const uint32 splitAxis,
                                  std::vector<std::shared_ptr<Intersector> > &subIntersectors1,
                                  std::vector<std::shared_ptr<Intersector> > &subIntersectors2) const {
    uint64 size = (intersectors.size() + 1) / 2;
    subIntersectors1.reserve(size);
    subIntersectors2.reserve(size);

    std::vector<std::shared_ptr<Intersector> > sortedIntersectors(intersectors);
    std::nth_element(sortedIntersectors.begin(),
                     sortedIntersectors.begin() + size,
                     sortedIntersectors.end(),
                     [splitAxis](std::shared_ptr<Intersector> i1, std::shared_ptr<Intersector> i2) {
                         return i1->bound().centroid()[splitAxis] < i2->bound().centroid()[splitAxis];
                     });

    subIntersectors1.insert(subIntersectors1.end(),
                            sortedIntersectors.begin(),
                            sortedIntersectors.begin() + size);

    subIntersectors2.insert(subIntersectors2.end(),
                            sortedIntersectors.begin() + size,
                            sortedIntersectors.end());
    return true;
}

} // namespace cadise