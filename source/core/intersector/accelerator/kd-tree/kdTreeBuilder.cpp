#include "core/intersector/accelerator/kd-tree/kdTreeBuilder.h"

#include "core/intersector/accelerator/kd-tree/endpoint.h"
#include "core/intersector/intersector.h"
#include "fundamental/assertion.h"
#include "math/tAabb.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace cadise {

KdTreeBuilder::KdTreeBuilder(
    const real traversalCost,
    const real intersectionCost,
    const real emptyBonus) :

    _traversalCost(traversalCost),
    _intersectionCost(intersectionCost),
    _emptyBonus(emptyBonus),
    _maxDepth(0) {
}

void KdTreeBuilder::buildNodes(
    const std::vector<std::shared_ptr<Intersector>>& intersectors,
    const std::vector<AABB3R>&                       intersectorBounds,
    const AABB3R&                                    entireBound,
    std::vector<KdTreeNode>* const                   out_nodes,
    std::vector<std::size_t>* const                  out_intersectorIndices) {

    CADISE_ASSERT(out_nodes);
    CADISE_ASSERT(out_intersectorIndices);

    if (intersectors.empty()) {
        return;
    }

    const std::size_t intersectorCounts = intersectors.size();
    _maxDepth = static_cast<std::size_t>(8 + 1.3_r * std::log2(intersectorCounts) + 0.5_r);

    std::vector<std::size_t> intersectorIndices(intersectorCounts);
    for (std::size_t i = 0; i < intersectorCounts; ++i) {
        intersectorIndices[i] = i;
    }

    _buildNodesRecursively(intersectorIndices,
                           intersectorBounds,
                           entireBound,
                           0,
                           0,
                           out_nodes,
                           out_intersectorIndices);
}

void KdTreeBuilder::_buildNodesRecursively(
    const std::vector<std::size_t>& intersectorIndices,
    const std::vector<AABB3R>&      intersectorBounds,
    const AABB3R&                   entireBound,
    const std::size_t               currentDepth,
    const std::size_t               badRefines,
    std::vector<KdTreeNode>* const  out_nodes,
    std::vector<std::size_t>* const out_intersectorIndices) const {

    CADISE_ASSERT(out_nodes);
    CADISE_ASSERT(out_intersectorIndices);

    KdTreeNode node;
    const std::size_t nodeIndex                    = out_nodes->size();
    const std::size_t intersectorIndicesBeginIndex = out_intersectorIndices->size();

    // make leaf node
    const std::size_t intersectorCounts = intersectorIndices.size();
    if (intersectorCounts <= MAX_INTERSECTOR_SIZE ||
        currentDepth == _maxDepth) {

        for (std::size_t i = 0; i < intersectorCounts; ++i) {
            out_intersectorIndices->push_back(intersectorIndices[i]);
        }

        node.initializeLeafNode(intersectorIndicesBeginIndex, intersectorCounts);
        out_nodes->push_back(std::move(node));
    }

    // make internal node if split is needed
    else {
        // decide which axis to split and where to split,
        // using SAH technique for each axis's each candidate
        // to find best split position (it means lowest cost).
        //
        // if split succeeds, intersectorIndices will be split 
        // into two sub-arrays.
        std::vector<std::size_t> subIntersectorIndicesA;
        std::vector<std::size_t> subIntersectorIndicesB;
        subIntersectorIndicesA.reserve(intersectorCounts);
        subIntersectorIndicesB.reserve(intersectorCounts);

        // it stores split axis and split position
        std::tuple<std::size_t, real> splitInfo;

        // make leaf node if there isn't good split choice
        std::size_t newBadRefines;
        if (!_canSplitWithSah(intersectorIndices,
                              intersectorBounds,
                              entireBound,
                              badRefines,
                              &newBadRefines,
                              &splitInfo,
                              &subIntersectorIndicesA,
                              &subIntersectorIndicesB)) {

            for (std::size_t i = 0; i < intersectorCounts; ++i) {
                out_intersectorIndices->push_back(intersectorIndices[i]);
            }

            node.initializeLeafNode(intersectorIndicesBeginIndex, intersectorCounts);
            out_nodes->push_back(std::move(node));
        }

        // make internal node
        else {
            std::size_t splitAxis;
            real        splitPosition;
            std::tie(splitAxis, splitPosition) = splitInfo;

            Vector3R splitBoundMinVertex = entireBound.minVertex();
            Vector3R splitBoundMaxVertex = entireBound.maxVertex();
            splitBoundMinVertex[splitAxis] = splitPosition;
            splitBoundMaxVertex[splitAxis] = splitPosition;

            const AABB3R splitBoundA(entireBound.minVertex(), splitBoundMaxVertex);
            const AABB3R splitBoundB(splitBoundMinVertex, entireBound.maxVertex());

            // make node first, and then initialize its data (internal node data)
            out_nodes->push_back(std::move(node));

            _buildNodesRecursively(subIntersectorIndicesA,
                                   intersectorBounds,
                                   splitBoundA,
                                   currentDepth + 1,
                                   newBadRefines,
                                   out_nodes,
                                   out_intersectorIndices);

            const std::size_t secondChildIndex = out_nodes->size();
            (*out_nodes)[nodeIndex].initializInternalNode(secondChildIndex, 
                                                          splitAxis, 
                                                          splitPosition);

            _buildNodesRecursively(subIntersectorIndicesB,
                                   intersectorBounds,
                                   splitBoundB,
                                   currentDepth + 1,
                                   newBadRefines,
                                   out_nodes,
                                   out_intersectorIndices);
        }
    }
}

bool KdTreeBuilder::_canSplitWithSah(
    const std::vector<std::size_t>&      intersectorIndices,
    const std::vector<AABB3R>&           intersectorBounds,
    const AABB3R&                        entireBound,
    const std::size_t                    badRefines,
    std::size_t* const                   out_newBadRefines,
    std::tuple<std::size_t, real>* const out_splitInfo,
    std::vector<std::size_t>* const      out_subIntersectorIndicesA,
    std::vector<std::size_t>* const      out_subIntersectorIndicesB) const {

    CADISE_ASSERT(out_newBadRefines);
    CADISE_ASSERT(out_splitInfo);
    CADISE_ASSERT(out_subIntersectorIndicesA);
    CADISE_ASSERT(out_subIntersectorIndicesB);

    // clear buffer first
    out_subIntersectorIndicesA->clear();
    out_subIntersectorIndicesA->shrink_to_fit();
    out_subIntersectorIndicesB->clear();
    out_subIntersectorIndicesB->shrink_to_fit();

    const std::size_t intersectorCounts  = intersectorIndices.size();
    const Vector3R    boundExtent        = entireBound.extent();
    const real        noSplitCost        = _intersectionCost * intersectorCounts;
    const real        inverseSurfaceArea = 1.0_r / entireBound.surfaceArea();

    std::size_t bestAxis          = std::numeric_limits<std::size_t>::max();
    std::size_t bestEndpointIndex = std::numeric_limits<std::size_t>::max();
    real        bestCost          = std::numeric_limits<real>::max();

    // build endpoints cache
    std::unique_ptr<Endpoint[]> endpoints[3];
    for (std::size_t i = 0; i < 3; ++i) {
        endpoints[i].reset(new Endpoint[2 * intersectorCounts]);
    }

    // first test split along the axis with maximum spatial extent
    std::size_t axis = boundExtent.maxDimension();

    std::size_t retryTimes = 0;
    while(bestAxis == std::numeric_limits<std::size_t>::max() && 
          retryTimes < 3) {

        // build split candidate lists
        for (std::size_t i = 0; i < intersectorCounts; ++i) {
            const std::size_t intersectorIndex = intersectorIndices[i];
            const AABB3R& bound = intersectorBounds[intersectorIndex];

            const std::size_t endpointBaseIndex = 2 * i;
            endpoints[axis][endpointBaseIndex + 0] = Endpoint(intersectorIndex, 
                                                              bound.minVertex()[axis],
                                                              EEndpointType::MIN);

            endpoints[axis][endpointBaseIndex + 1] = Endpoint(intersectorIndex, 
                                                              bound.maxVertex()[axis],
                                                              EEndpointType::MAX);
        }

        // sort all enpoints according to their positions and types
        std::sort(
            &(endpoints[axis][0]), 
            &(endpoints[axis][2 * intersectorCounts]),
            [](const Endpoint& eA, const Endpoint& eB) {
                if (eA.position() == eB.position()) {
                    return eA.type() < eB.type();
                }
                else {
                    return eA.position() < eB.position();
                }
            });

        std::size_t subIntersectorCountsA = 0;
        std::size_t subIntersectorCountsB = intersectorCounts;

        // test for each split candidate
        for (std::size_t i = 0; i < 2 * intersectorCounts; ++i) {
            if (endpoints[axis][i].type() == EEndpointType::MAX) {
                --subIntersectorCountsB;
            }

            // calculate split cost for i-th split candidate
            // if it is within the bound
            const real splitPosition = endpoints[axis][i].position();
            if (splitPosition > entireBound.minVertex()[axis] &&
                splitPosition < entireBound.maxVertex()[axis]) {

                Vector3R splitBoundMinVertex = entireBound.minVertex();
                Vector3R splitBoundMaxVertex = entireBound.maxVertex();
                splitBoundMinVertex[axis] = splitPosition;
                splitBoundMaxVertex[axis] = splitPosition;

                const real probabilitySplitBoundA
                    = AABB3R(entireBound.minVertex(), splitBoundMaxVertex).surfaceArea() * inverseSurfaceArea;
                const real probabilitySplitBoundB
                    = AABB3R(splitBoundMinVertex, entireBound.maxVertex()).surfaceArea() * inverseSurfaceArea;

                const real emptyBonus = (subIntersectorCountsA == 0 || subIntersectorCountsB == 0) ?
                                        _emptyBonus : 0.0_r;

                const real splitCost
                    = _traversalCost + _intersectionCost * (1.0_r - emptyBonus) *
                      (probabilitySplitBoundA * subIntersectorCountsA + probabilitySplitBoundB * subIntersectorCountsB);

                if (splitCost < bestCost) {
                    bestCost          = splitCost;
                    bestAxis          = axis;
                    bestEndpointIndex = i;
                }
            }

            if (endpoints[axis][i].type() == EEndpointType::MIN) {
                ++subIntersectorCountsA;
            }
        }

        axis = (axis + 1) % 3;
        ++retryTimes;
    }

    std::size_t newBadRefines = badRefines;
    if (bestCost > noSplitCost) {
        ++newBadRefines;
    }

    // it could not find good split position
    if ((bestCost > 4.0_r * noSplitCost && intersectorCounts < 16) ||
        bestAxis == std::numeric_limits<std::size_t>::max() ||
        newBadRefines == 3) {

        return false;
    }

    *out_newBadRefines = newBadRefines;
    *out_splitInfo = std::make_tuple(bestAxis, endpoints[bestAxis][bestEndpointIndex].position());

    for (std::size_t i = 0; i < bestEndpointIndex; ++i) {
        if (endpoints[bestAxis][i].type() == EEndpointType::MIN) {
            out_subIntersectorIndicesA->push_back(endpoints[bestAxis][i].intersectorIndex());
        }
    }

    for (std::size_t i = bestEndpointIndex; i < 2 * intersectorCounts; ++i) {
        if (endpoints[bestAxis][i].type() == EEndpointType::MAX) {
            out_subIntersectorIndicesB->push_back(endpoints[bestAxis][i].intersectorIndex());
        }
    }

    return true;
}

} // namespace cadise