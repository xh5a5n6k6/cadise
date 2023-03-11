#include "Core/Intersector/Accelerator/KDTree/KDTreeAccelerator.h"

#include "Core/Intersector/Accelerator/KDTree/KDTreeBuilder.h"
#include "Core/Intersector/Accelerator/KDTree/KDTreeNodeInfo.h"
#include "Core/Ray.h"
#include "Foundation/Assertion.h"

namespace cadise 
{

KDTreeAccelerator::KDTreeAccelerator(
    const std::vector<std::shared_ptr<Intersector>>& intersectors,
    const real                                       traversalCost,
    const real                                       intersectionCost,
    const real                                       emptyBonus) :

    Accelerator(),
    _intersectors(std::move(intersectors)),
    _nodes(),
    _intersectorIndices(),
    _bound() 
{
    // calculate AABBs of all intersectors
    AABB3R bound;
    std::vector<AABB3R> intersectorBounds(_intersectors.size());
    for (std::size_t i = 0; i < _intersectors.size(); ++i) 
    {
        _intersectors[i]->evaluateBound(&bound);

        intersectorBounds[i] = bound;
        _bound.unionWithLocal(bound);
    }

    KDTreeBuilder builder(traversalCost, intersectionCost, emptyBonus);
    builder.buildNodes(
        _intersectors, 
        intersectorBounds, 
        _bound, 
        &_nodes, 
        &_intersectorIndices);
}

void KDTreeAccelerator::evaluateBound(AABB3R* const out_bound) const
{
    CS_ASSERT(out_bound);

    out_bound->set(_bound);
}

bool KDTreeAccelerator::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const
{
    bool result = false;

    const Vector3R& origin       = ray.origin();
    const Vector3R& direction    = ray.direction();
    const Vector3R  rcpDirection = direction.reciprocal();

    real boundMinT;
    real boundMaxT;
    if (!_bound.isIntersectingAABB(origin, 
                                   rcpDirection, 
                                   ray.minT(), 
                                   ray.maxT(), 
                                   &boundMinT, 
                                   &boundMaxT)) 
    {
        return result;
    }

    KDTreeNodeInfo currentNodeInfo(0, boundMinT, boundMaxT);
    std::size_t    currentStackSize = 0;
    KDTreeNodeInfo nodeInfoStack[MAX_STACK_SIZE];

    while (true) 
    {
        const std::size_t currentNodeIndex = currentNodeInfo.nodeIndex();
        const KDTreeNode& currentNode      = _nodes[currentNodeIndex];

        // early exits when there is a closer intersection
        // than current node
        if (ray.maxT() < currentNodeInfo.minT()) 
        {
            break;
        }

        // leaf node traversal
        if (currentNode.isLeaf()) 
        {
            for (std::size_t i = 0; i < currentNode.numObjects(); ++i) 
            {
                const std::size_t realIntersectorIndex 
                    = _intersectorIndices[currentNode.objectIndex() + i];

                result |= _intersectors[realIntersectorIndex]->isIntersecting(ray, primitiveInfo);
            }

            if (currentStackSize == 0) 
            {
                break;
            }
            else 
            {
                --currentStackSize;
                currentNodeInfo = nodeInfoStack[currentStackSize];
            }

        } // end leaf node traversal

        // internal node traversal
        else 
        {
            const std::size_t splitAxis = currentNode.splitAxis();
            const real splitPlaneT 
                = (currentNode.splitPosition() - origin[splitAxis]) * rcpDirection[splitAxis];

            std::size_t nearChildIndex;
            std::size_t farChildIndex;

            // decide which child is near/far node
            //
            // near child is first node
            if ((origin[splitAxis] < currentNode.splitPosition()) ||
                (origin[splitAxis] == currentNode.splitPosition() && direction[splitAxis] <= 0.0_r)) 
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

            // there are three cases for internal nodes traversal,
            // first two cases don't need to change minT/maxT
            // in currentNodeInfo

            // CaseI: only near child node is hit
            if (currentNodeInfo.maxT() < splitPlaneT || splitPlaneT <= 0.0_r)
            {
                currentNodeInfo.setNodeIndex(nearChildIndex);
            }
            // CaseII: only far child node is hit
            else if (splitPlaneT < currentNodeInfo.minT())
            {
                currentNodeInfo.setNodeIndex(farChildIndex);
            }
            // CaseIII: both children nodes are hit
            else
            {
                // push far child node in the stack
                nodeInfoStack[currentStackSize].setNodeIndex(farChildIndex);
                nodeInfoStack[currentStackSize].setMinT(splitPlaneT);
                nodeInfoStack[currentStackSize].setMaxT(currentNodeInfo.maxT());
                ++currentStackSize;

                currentNodeInfo.setNodeIndex(nearChildIndex);
                currentNodeInfo.setMaxT(splitPlaneT);
            }
        } // end internal node traversal
    } // end while loop

    return result;
}

bool KDTreeAccelerator::isOccluded(const Ray& ray) const
{
    const Vector3R& origin       = ray.origin();
    const Vector3R& direction    = ray.direction();
    const Vector3R  rcpDirection = direction.reciprocal();

    real boundMinT;
    real boundMaxT;
    if (!_bound.isIntersectingAABB(origin,
                                   rcpDirection,
                                   ray.minT(),
                                   ray.maxT(),
                                   &boundMinT,
                                   &boundMaxT)) 
    {
        return false;
    }

    KDTreeNodeInfo currentNodeInfo(0, boundMinT, boundMaxT);
    std::size_t    currentStackSize = 0;
    KDTreeNodeInfo nodeInfoStack[MAX_STACK_SIZE];

    while (true)
    {
        const std::size_t currentNodeIndex = currentNodeInfo.nodeIndex();
        const KDTreeNode& currentNode      = _nodes[currentNodeIndex];

        // early exits when there is a closer intersection
        // than current node
        if (ray.maxT() < currentNodeInfo.minT()) 
        {
            break;
        }

        // leaf node traversal
        if (currentNode.isLeaf())
        {
            for (std::size_t i = 0; i < currentNode.numObjects(); ++i) 
            {
                const std::size_t realIntersectorIndex 
                    = _intersectorIndices[currentNode.objectIndex() + i];

                if (_intersectors[realIntersectorIndex]->isOccluded(ray))
                {
                    return true;
                }
            }

            if (currentStackSize == 0) 
            {
                break;
            }
            else 
            {
                --currentStackSize;
                currentNodeInfo = nodeInfoStack[currentStackSize];
            }

        } // end leaf node traversal

        // internal node traversal
        else 
        {
            const std::size_t splitAxis = currentNode.splitAxis();
            const real splitPlaneT 
                = (currentNode.splitPosition() - origin[splitAxis]) * rcpDirection[splitAxis];

            std::size_t nearChildIndex;
            std::size_t farChildIndex;

            // decide which child is near/far node
            //
            // near child is first node
            if ((origin[splitAxis] < currentNode.splitPosition()) ||
                (origin[splitAxis] == currentNode.splitPosition() && direction[splitAxis] <= 0.0_r))
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

            // there are three cases for internal nodes traversal,
            // first two cases don't need to change minT/maxT
            // in currentNodeInfo

            // CaseI: only near child node is hit
            if (currentNodeInfo.maxT() < splitPlaneT || splitPlaneT <= 0.0_r) 
            {
                currentNodeInfo.setNodeIndex(nearChildIndex);
            }
            // CaseII: only far child node is hit
            else if (splitPlaneT < currentNodeInfo.minT()) 
            {
                currentNodeInfo.setNodeIndex(farChildIndex);
            }
            // CaseIII: both children nodes are hit
            else 
            {
                // push far child node in the stack
                nodeInfoStack[currentStackSize].setNodeIndex(farChildIndex);
                nodeInfoStack[currentStackSize].setMinT(splitPlaneT);
                nodeInfoStack[currentStackSize].setMaxT(currentNodeInfo.maxT());
                ++currentStackSize;

                currentNodeInfo.setNodeIndex(nearChildIndex);
                currentNodeInfo.setMaxT(splitPlaneT);
            }
        } // end internal node traversal
    } // end while loop

    return false;
}

} // namespace cadise