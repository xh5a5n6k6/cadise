#include "core/intersector/accelerator/bvh/bvhBuilder.h"

#include "core/intersector/accelerator/bvh/bvhBinaryNode.h"
#include "core/intersector/accelerator/bvh/bvhBoundInfo.h"
#include "core/intersector/accelerator/bvh/bvhSahBucket.h"
#include "core/intersector/intersector.h"
#include "fundamental/assertion.h"

#include <algorithm>
#include <limits>

namespace cadise {

BvhBuilder::BvhBuilder(const BvhSplitMode& splitMode) :
    _splitMode(splitMode) {
}

std::unique_ptr<BvhBinaryNode> BvhBuilder::buildBinaryNodes(
    const std::vector<std::shared_ptr<Intersector>>& intersectors, 
    std::vector<std::shared_ptr<Intersector>>* const out_orderedIntersectors,
    std::size_t* const                               out_totalNodeSize) const {

    CADISE_ASSERT(out_orderedIntersectors);
    CADISE_ASSERT(out_totalNodeSize);

    const std::size_t intersectorCounts = intersectors.size();

    std::vector<BvhBoundInfo> boundInfos;
    boundInfos.reserve(intersectorCounts);
    AABB3R bound;
    for (std::size_t i = 0; i < intersectorCounts; ++i) {
        intersectors[i]->evaluateBound(&bound);
        boundInfos.push_back(BvhBoundInfo(bound, i));
    }

    std::unique_ptr<BvhBinaryNode> root = nullptr;
    root = _buildBinaryNodesRecursively(boundInfos,
                                        intersectors, 
                                        out_orderedIntersectors, 
                                        out_totalNodeSize);

    return std::move(root);
}

void BvhBuilder::buildLinearNodes(std::unique_ptr<BvhBinaryNode>    root, 
                                  const std::size_t                 totalNodeSize,
                                  std::vector<BvhLinearNode>* const out_linearNodes) const {
    
    CADISE_ASSERT(out_linearNodes);
    
    std::vector<BvhLinearNode> nodes;
    nodes.reserve(totalNodeSize);

    _buildLinearNodesRecursively(std::move(root), &nodes, nullptr);

    out_linearNodes->swap(nodes);
}

std::unique_ptr<BvhBinaryNode> BvhBuilder::_buildBinaryNodesRecursively(
    const std::vector<BvhBoundInfo>&                 boundInfos,
    const std::vector<std::shared_ptr<Intersector>>& intersectors, 
    std::vector<std::shared_ptr<Intersector>>* const out_orderedIntersectors,
    std::size_t* const                               out_totalNodeSize) const {

    CADISE_ASSERT(out_orderedIntersectors);
    CADISE_ASSERT(out_totalNodeSize);

    std::unique_ptr<BvhBinaryNode> node = std::make_unique<BvhBinaryNode>();

    const std::size_t intersectorBeginIndex = out_orderedIntersectors->size();
    const std::size_t intersectorCounts     = boundInfos.size();

    AABB3R nodeBound;
    for (std::size_t i = 0; i < intersectorCounts; ++i) {
        nodeBound.unionWith(boundInfos[i].bound());
    }

    // make leaf node
    if (intersectorCounts <= MAX_INTERSECTOR_SIZE) {
        for (std::size_t i = 0; i < intersectorCounts; ++i) {
            const std::size_t intersectorIndex = boundInfos[i].index();
            out_orderedIntersectors->push_back(intersectors[intersectorIndex]);
        }

        node->initializeLeafNode(nodeBound, intersectorBeginIndex, intersectorCounts);
    }

    // make internal node if split succeeds, or make leaf node
    else {
        AABB3R centroidBound;
        for (std::size_t i = 0; i < intersectorCounts; ++i) {
            centroidBound.unionWith(boundInfos[i].centroid());
        }

        const std::size_t splitAxis = centroidBound.maxAxis();
        std::vector<BvhBoundInfo> subBoundInfosA;
        std::vector<BvhBoundInfo> subBoundInfosB;

        bool canSplit = false;
        switch (_splitMode) {
            case BvhSplitMode::EQUAL:
                canSplit = _canSplitWithEqual(boundInfos,
                                              splitAxis,
                                              &subBoundInfosA, 
                                              &subBoundInfosB);
                break;

            case BvhSplitMode::SAH:
                canSplit = _canSplitWithSah(boundInfos,
                                            splitAxis,
                                            nodeBound,
                                            centroidBound,
                                            &subBoundInfosA,
                                            &subBoundInfosB);
                break;

            default:
                break;
        }

        // make leaf node
        if(!canSplit){
            for (std::size_t i = 0; i < intersectorCounts; ++i) {
                const std::size_t intersectorIndex = boundInfos[i].index();
                out_orderedIntersectors->push_back(intersectors[intersectorIndex]);
            }

            node->initializeLeafNode(nodeBound, intersectorBeginIndex, intersectorCounts);
        }

        // make internal node
        else {
            std::unique_ptr<BvhBinaryNode> firstChild = nullptr;
            firstChild = _buildBinaryNodesRecursively(subBoundInfosA,
                                                      intersectors,
                                                      out_orderedIntersectors,
                                                      out_totalNodeSize);

            std::unique_ptr<BvhBinaryNode> secondChild = nullptr;
            secondChild = _buildBinaryNodesRecursively(subBoundInfosB,
                                                       intersectors,
                                                       out_orderedIntersectors,
                                                       out_totalNodeSize);

            node->initializeInternalNode(nodeBound,
                                         std::move(firstChild),
                                         std::move(secondChild),
                                         splitAxis);
        }
    }

    *out_totalNodeSize += 1;

    return std::move(node);
}

void BvhBuilder::_buildLinearNodesRecursively(
    std::unique_ptr<BvhBinaryNode>    binaryNode, 
    std::vector<BvhLinearNode>* const out_linearNodes,
    std::size_t* const                out_secondChildNodeIndex) const {
    
    CADISE_ASSERT(out_linearNodes);
    
    BvhLinearNode linearNode;
    const std::size_t nodeIndex = out_linearNodes->size();
    if (out_secondChildNodeIndex) {
        *out_secondChildNodeIndex = nodeIndex;
    }

    if (binaryNode->isLeaf()) {
        linearNode.initializeLeafNode(binaryNode->bound(), 
                                      binaryNode->intersectorIndex(), 
                                      binaryNode->intersectorCounts());

        out_linearNodes->push_back(linearNode);
    }
    else {
        out_linearNodes->push_back(linearNode);
        
        const AABB3R&     internalNodeBound     = binaryNode->bound();
        const std::size_t internalNodeSplitAxis = binaryNode->splitAxis();
        std::size_t secondChildIndex;

        _buildLinearNodesRecursively(std::move(binaryNode->firstChild()), 
                                     out_linearNodes, 
                                     nullptr);

        _buildLinearNodesRecursively(std::move(binaryNode->secondChild()), 
                                     out_linearNodes, 
                                     &secondChildIndex);

        (*out_linearNodes)[nodeIndex].initializeInternalNode(internalNodeBound, 
                                                             secondChildIndex, 
                                                             internalNodeSplitAxis);
    }
}

bool BvhBuilder::_canSplitWithEqual(
    const std::vector<BvhBoundInfo>& boundInfos,
    const std::size_t                splitAxis,
    std::vector<BvhBoundInfo>* const out_subBoundInfosA,
    std::vector<BvhBoundInfo>* const out_subBoundInfosB) const {

    CADISE_ASSERT(out_subBoundInfosA);
    CADISE_ASSERT(out_subBoundInfosB);

    const std::size_t size = (boundInfos.size() + 1) / 2;
    out_subBoundInfosA->reserve(size);
    out_subBoundInfosB->reserve(size);

    std::vector<BvhBoundInfo> sortedBoundInfos(boundInfos);
    std::nth_element(
        sortedBoundInfos.begin(),
        sortedBoundInfos.begin() + size,
        sortedBoundInfos.end(),
        [splitAxis](const BvhBoundInfo& bA, const BvhBoundInfo& bB) {
            return bA.centroid()[splitAxis] < bB.centroid()[splitAxis];
        });

    out_subBoundInfosA->insert(out_subBoundInfosA->end(),
                               sortedBoundInfos.begin(),
                               sortedBoundInfos.begin() + size);

    out_subBoundInfosB->insert(out_subBoundInfosB->end(),
                               sortedBoundInfos.begin() + size,
                               sortedBoundInfos.end());

    return true;
}

bool BvhBuilder::_canSplitWithSah(
    const std::vector<BvhBoundInfo>& boundInfos,
    const std::size_t                splitAxis,
    const AABB3R&                    intersectorBound,
    const AABB3R&                    centroidBound,
    std::vector<BvhBoundInfo>* const out_subBoundInfosA,
    std::vector<BvhBoundInfo>* const out_subBoundInfosB) const {

    CADISE_ASSERT(out_subBoundInfosA);
    CADISE_ASSERT(out_subBoundInfosB);

    // clear buffer first
    out_subBoundInfosA->clear();
    out_subBoundInfosA->shrink_to_fit();
    out_subBoundInfosB->clear();
    out_subBoundInfosB->shrink_to_fit();

    const std::size_t intersectorCounts  = boundInfos.size();
    const Vector3R&   splitMinVertex     = centroidBound.minVertex();
    const real        inverseSplitExtent = 1.0_r / centroidBound.extent()[splitAxis];

    constexpr std::size_t numBuckets = 12;
    BvhSahBucket buckets[numBuckets];
    for (std::size_t i = 0; i < intersectorCounts; ++i) {
        const AABB3R&   bound    = boundInfos[i].bound();
        const Vector3R& centroid = boundInfos[i].centroid();
        const real      offset   = (centroid[splitAxis] - splitMinVertex[splitAxis]) * inverseSplitExtent;

        const std::size_t bucketIndex     = static_cast<std::size_t>(offset * numBuckets);
        const std::size_t safeBucketIndex = (bucketIndex == numBuckets) ? numBuckets - 1 : bucketIndex;

        buckets[safeBucketIndex].addOneBoundAndCount(bound);
    }

    constexpr real traversalCost      = 0.125_r;
    constexpr real intersectionCost   = 1.0_r;
    const     real noSplitCost        = intersectionCost * intersectorCounts;
    const     real inverseSurfaceArea = 1.0_r / intersectorBound.surfaceArea();

    std::size_t bestSplitBucketIndex = std::numeric_limits<std::size_t>::max();
    real        bestCost             = std::numeric_limits<real>::max();

    for (std::size_t split = 0; split < numBuckets - 1; ++split) {
        std::size_t subIntersectorCountsA = 0;
        std::size_t subIntersectorCountsB = 0;
        AABB3R      subBoundA;
        AABB3R      subBoundB;

        for (std::size_t i = 0; i <= split; ++i) {
            subBoundA.unionWith(buckets[i].bound());
            subIntersectorCountsA += buckets[i].intersectorCounts();
        }

        for (std::size_t i = split + 1; i < numBuckets; ++i) {
            subBoundB.unionWith(buckets[i].bound());
            subIntersectorCountsB += buckets[i].intersectorCounts();
        }

        const real probabilitySplitBoundA = subBoundA.surfaceArea() * inverseSurfaceArea;
        const real probabilitySplitBoundB = subBoundB.surfaceArea() * inverseSurfaceArea;

        const real splitCost 
            = traversalCost + intersectionCost * 
              (probabilitySplitBoundA * subIntersectorCountsA + probabilitySplitBoundB * subIntersectorCountsB);

        if (splitCost < bestCost) {
            bestCost = splitCost;
            bestSplitBucketIndex = split;
        }
    }

    // it could not find good split position
    if (bestCost >= noSplitCost) {
        return false;
    }

    std::vector<BvhBoundInfo> sortedBoundInfos(boundInfos);
    const auto middlePointer = std::partition(
        sortedBoundInfos.begin(),
        sortedBoundInfos.end(),
        [=](const BvhBoundInfo& b) {
            const Vector3R& centroid = b.centroid();
            const real      offset   = (centroid[splitAxis] - splitMinVertex[splitAxis]) * inverseSplitExtent;

            const std::size_t bucketIndex     = static_cast<std::size_t>(offset * numBuckets);
            const std::size_t safeBucketIndex = (bucketIndex == numBuckets) ? numBuckets - 1 : bucketIndex;

            return safeBucketIndex <= bestSplitBucketIndex;
        });

    out_subBoundInfosA->insert(out_subBoundInfosA->end(),
                               sortedBoundInfos.begin(),
                               middlePointer);

    out_subBoundInfosB->insert(out_subBoundInfosB->end(),
                               middlePointer,
                               sortedBoundInfos.end());

    return true;
}

} // namespace cadise