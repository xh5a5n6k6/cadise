#pragma once

#include "Core/Gear/TPointKDTree.h"

#include "Foundation/Assertion.h"

#include <algorithm>

namespace cadise
{

template<typename Index, typename Object, typename ObjectCenterCalculator>
inline TPointKDTree<Index, Object, ObjectCenterCalculator>::TPointKDTree(
    const ObjectCenterCalculator& centerCalculator) :

    _objects(),
    _nodes(),
    _objectIndices(),
    _bound(),
    _centerCalculator(centerCalculator)
{}

template<typename Index, typename Object, typename ObjectCenterCalculator>
inline void TPointKDTree<Index, Object, ObjectCenterCalculator>::
    buildNodes(const std::vector<Object>& objects)
{
    // clear buffer first
    _objects.clear();
    _nodes.clear();
    _objectIndices.clear();
    _bound.reset();

    _objects = std::move(objects);

    std::vector<Vector3R> objectCenters(objects.size());
    for (std::size_t i = 0; i < objects.size(); ++i)
    {
        const Vector3R center = _centerCalculator(objects[i]);

        objectCenters[i] = center;
        _bound.unionWithLocal(center);
    }

    std::vector<Index> objectIndices(objects.size());
    for (std::size_t i = 0; i < objects.size(); ++i)
    {
        objectIndices[i] = static_cast<Index>(i);
    }

    _buildNodesRecursively(
        objectCenters,
        objectIndices,
        _bound,
        0);
}

template<typename Index, typename Object, typename ObjectCenterCalculator>
inline void TPointKDTree<Index, Object, ObjectCenterCalculator>::
    findWithRange(
        const Vector3R&            position,
        const real                 searchRadius,
        std::vector<Object>* const out_objects) const
{
    CS_ASSERT(out_objects);

    if (!_bound.isInside(position))
    {
        return;
    }

    const real searchRadius2 = searchRadius * searchRadius;

    std::size_t currentNodeIndex = 0;
    std::size_t currentStackSize = 0;
    std::size_t nodeStack[MAX_STACK_SIZE];

    while (true)
    {
        const Node& currentNode = _nodes[currentNodeIndex];

        // leaf node traversal
        if (currentNode.isLeaf())
        {
            for (std::size_t i = 0; i < currentNode.numObjects(); ++i)
            {
                const Index   objectIndex = _objectIndices[currentNode.objectIndex() + i];
                const Object& object      = _objects[objectIndex];

                const Vector3R center    = _centerCalculator(object);
                const real     distance2 = center.sub(position).lengthSquared();
                if (distance2 < searchRadius2)
                {
                    out_objects->push_back(object);
                }
            }

            if (currentStackSize == 0)
            {
                break;
            }
            else
            {
                --currentStackSize;
                currentNodeIndex = nodeStack[currentStackSize];
            }

        } // end leaf node traversal

        // internal node traversal
        else
        {
            const std::size_t splitAxis = currentNode.splitAxis();
            const real splitDifferenceT
                = position[splitAxis] - currentNode.splitPosition();

            std::size_t nearChildIndex;
            std::size_t farChildIndex;

            // decide which child is near/far node
            //
            // near child is first node
            if (splitDifferenceT < 0.0_r)
            {
                nearChildIndex = currentNodeIndex + 1;
                farChildIndex  = currentNode.secondChildIndex();
            }
            // near child is second node
            else
            {
                nearChildIndex = currentNode.secondChildIndex();
                farChildIndex  = currentNodeIndex + 1;
            }

            // check if it needs to put farChildIndex in the stack
            if (splitDifferenceT * splitDifferenceT <= searchRadius2)
            {
                nodeStack[currentStackSize] = farChildIndex;
                ++currentStackSize;
            }

            currentNodeIndex = nearChildIndex;
        } // end internal node traversal
    } // end while loop
}

template<typename Index, typename Object, typename ObjectCenterCalculator>
inline void TPointKDTree<Index, Object, ObjectCenterCalculator>::
    _buildNodesRecursively(
        const std::vector<Vector3R>& objectCenters,
        const std::vector<Index>&    objectIndices,
        const AABB3R&                entireBound,
        const std::size_t            currentDepth)
{
    Node node;
    const std::size_t nodeIndex               = _nodes.size();
    const std::size_t objectIndicesBeginIndex = _objectIndices.size();

    // make leaf node
    const std::size_t numObjects = objectIndices.size();
    if (numObjects <= MAX_OBJECT_SIZE)
    {
        for (std::size_t i = 0; i < numObjects; ++i)
        {
            _objectIndices.push_back(objectIndices[i]);
        }

        node.initializeLeafNode(objectIndicesBeginIndex, numObjects);
        _nodes.push_back(std::move(node));
    }

    // make internal node
    else
    {
        // if split succeeds, objectIndices will be split 
        // into two sub-arrays.
        std::vector<Index> subObjectIndicesA;
        std::vector<Index> subObjectIndicesB;

        // it stores split axis and split position
        std::tuple<std::size_t, real> splitInfo;

        // make leaf node if there isn't good split choice
        if (!_canSplitWithEqual(
            objectCenters,
            objectIndices,
            entireBound,
            &splitInfo,
            &subObjectIndicesA,
            &subObjectIndicesB))
        {
            for (std::size_t i = 0; i < numObjects; ++i)
            {
                _objectIndices.push_back(objectIndices[i]);
            }

            node.initializeLeafNode(objectIndicesBeginIndex, numObjects);
            _nodes.push_back(std::move(node));
        }

        // make internal node
        else
        {
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
            _nodes.push_back(std::move(node));

            _buildNodesRecursively(
                objectCenters,
                subObjectIndicesA,
                splitBoundA,
                currentDepth + 1);

            const std::size_t secondChildIndex = _nodes.size();
            _nodes[nodeIndex].initializInternalNode(
                secondChildIndex,
                splitAxis,
                splitPosition);

            _buildNodesRecursively(
                objectCenters,
                subObjectIndicesB,
                splitBoundB,
                currentDepth + 1);
        }
    }
}

template<typename Index, typename Object, typename ObjectCenterCalculator>
inline bool TPointKDTree<Index, Object, ObjectCenterCalculator>::
    _canSplitWithEqual(
        const std::vector<Vector3R>&         objectCenters,
        const std::vector<Index>&            objectIndices,
        const AABB3R&                        entireBound,
        std::tuple<std::size_t, real>* const out_splitInfo,
        std::vector<Index>* const            out_subObjectIndicesA,
        std::vector<Index>* const            out_subObjectIndicesB)
{
    CS_ASSERT(out_subObjectIndicesA);
    CS_ASSERT(out_subObjectIndicesB);

    const std::size_t splitAxis = entireBound.maxAxis();

    const std::size_t size = (objectIndices.size() + 1) / 2;
    out_subObjectIndicesA->reserve(size);
    out_subObjectIndicesB->reserve(size);

    std::vector<Index> sortedObjectIndices(objectIndices);
    std::nth_element(
        sortedObjectIndices.begin(),
        sortedObjectIndices.begin() + size,
        sortedObjectIndices.end(),
        [splitAxis, &objectCenters](const Index& iA, const Index& iB)
        {
            return objectCenters[iA][splitAxis] < objectCenters[iB][splitAxis];
        });

    out_subObjectIndicesA->insert(
        out_subObjectIndicesA->end(),
        sortedObjectIndices.begin(),
        sortedObjectIndices.begin() + size);

    out_subObjectIndicesB->insert(
        out_subObjectIndicesB->end(),
        sortedObjectIndices.begin() + size,
        sortedObjectIndices.end());

    *out_splitInfo = std::make_tuple(splitAxis, objectCenters[sortedObjectIndices[size]][splitAxis]);

    return true;
}

} // namespace cadise