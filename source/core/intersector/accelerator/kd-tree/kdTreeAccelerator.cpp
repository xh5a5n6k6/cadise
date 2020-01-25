#include "core/intersector/accelerator/kd-tree/kdTreeAccelerator.h"

#include "core/intersector/accelerator/kd-tree/kdTreeBuilder.h"
#include "core/intersector/accelerator/kd-tree/kdTreeNodeInfo.h"
#include "core/ray.h"
#include "fundamental/assertion.h"

namespace cadise {

KdTreeAccelerator::KdTreeAccelerator(
    const std::vector<std::shared_ptr<Intersector>>& intersectors,
    const real                                       traversalCost,
    const real                                       intersectionCost,
    const real                                       emptyBonus) :

    _intersectors(std::move(intersectors)),
    _nodes(),
    _intersectorIndices(),
    _bound() {

    // calculate AABBs of all intersectors
    AABB3R bound;
    std::vector<AABB3R> intersectorBounds(_intersectors.size());
    for (std::size_t i = 0; i < _intersectors.size(); ++i) {
        _intersectors[i]->evaluateBound(&bound);
        intersectorBounds[i] = bound;
        _bound.unionWith(bound);
    }

    KdTreeBuilder builder(traversalCost, intersectionCost, emptyBonus);
    builder.buildNodes(_intersectors, 
                       intersectorBounds, 
                       _bound, 
                       &_nodes, 
                       &_intersectorIndices);
}

void KdTreeAccelerator::evaluateBound(AABB3R* const out_bound) const {
    CADISE_ASSERT(out_bound);

    *out_bound = _bound;
}

bool KdTreeAccelerator::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const {
    bool result = false;

    const Vector3R& origin           = ray.origin();
    const Vector3R& direction        = ray.direction();
    const Vector3R  inverseDirection = direction.reciprocal();

    real boundMinT;
    real boundMaxT;
    if (!_bound.isIntersectingAABB(origin, 
                                   inverseDirection, 
                                   ray.minT(), 
                                   ray.maxT(), 
                                   &boundMinT, 
                                   &boundMaxT)) {
        return result;
    }

    KdTreeNodeInfo currentNodeInfo(0, boundMinT, boundMaxT);
    std::size_t    currentStackSize = 0;
    KdTreeNodeInfo nodeInfoStack[MAX_STACK_SIZE];

    while (true) {
        const std::size_t currentNodeIndex = currentNodeInfo.nodeIndex();
        const KdTreeNode& currentNode      = _nodes[currentNodeIndex];

        // early exits when there is a closer intersection
        // than current node
        if (ray.maxT() < currentNodeInfo.minT()) {
            break;
        }

        // leaf node traversal
        if (currentNode.isLeaf()) {
            for (std::size_t i = 0; i < currentNode.intersectorCounts(); ++i) {
                const std::size_t realIntersectorIndex 
                    = _intersectorIndices[currentNode.intersectorIndex() + i];

                result |= _intersectors[realIntersectorIndex]->isIntersecting(ray, primitiveInfo);
            }

            if (currentStackSize == 0) {
                break;
            }
            else {
                --currentStackSize;
                currentNodeInfo = nodeInfoStack[currentStackSize];
            }

        } // end leaf node traversal

        // internal node traversal
        else {
            const std::size_t splitAxis = currentNode.splitAxis();
            const real splitPlaneT 
                = (currentNode.splitPosition() - origin[splitAxis]) * inverseDirection[splitAxis];

            std::size_t nearChildIndex;
            std::size_t farChildIndex;

            // decide which child is near/far node
            // near child is first node
            if ((origin[splitAxis] < currentNode.splitPosition()) ||
                (origin[splitAxis] == currentNode.splitPosition() && direction[splitAxis] <= 0.0_r)) {

                nearChildIndex = currentNodeIndex + 1;
                farChildIndex  = currentNode.secondChildIndex();
            }
            // near child is second node
            else {
                nearChildIndex = currentNode.secondChildIndex();
                farChildIndex  = currentNodeIndex + 1;
            }

            // there are three cases for internal nodes traversal,
            // first two cases don't need to change minT/maxT
            // in currentNodeInfo

            // CaseI: only near child node is hit
            if (currentNodeInfo.maxT() < splitPlaneT || splitPlaneT <= 0.0_r) {
                currentNodeInfo.setNodeIndex(nearChildIndex);
            }
            // CaseII: only far child node is hit
            else if (splitPlaneT < currentNodeInfo.minT()) {
                currentNodeInfo.setNodeIndex(farChildIndex);
            }
            // CaseIII: both children nodes are hit
            else {
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

bool KdTreeAccelerator::isOccluded(const Ray& ray) const {
    const Vector3R& origin           = ray.origin();
    const Vector3R& direction        = ray.direction();
    const Vector3R  inverseDirection = direction.reciprocal();

    real boundMinT;
    real boundMaxT;
    if (!_bound.isIntersectingAABB(origin,
                                   inverseDirection,
                                   ray.minT(),
                                   ray.maxT(),
                                   &boundMinT,
                                   &boundMaxT)) {
        return false;
    }

    KdTreeNodeInfo currentNodeInfo(0, boundMinT, boundMaxT);
    std::size_t    currentStackSize = 0;
    KdTreeNodeInfo nodeInfoStack[MAX_STACK_SIZE];

    while (true) {
        const std::size_t currentNodeIndex = currentNodeInfo.nodeIndex();
        const KdTreeNode& currentNode      = _nodes[currentNodeIndex];

        // early exits when there is a closer intersection
        // than current node
        if (ray.maxT() < currentNodeInfo.minT()) {
            break;
        }

        // leaf node traversal
        if (currentNode.isLeaf()) {
            for (std::size_t i = 0; i < currentNode.intersectorCounts(); ++i) {
                const std::size_t realIntersectorIndex 
                    = _intersectorIndices[currentNode.intersectorIndex() + i];

                if (_intersectors[realIntersectorIndex]->isOccluded(ray)) {
                    return true;
                }
            }

            if (currentStackSize == 0) {
                break;
            }
            else {
                --currentStackSize;
                currentNodeInfo = nodeInfoStack[currentStackSize];
            }

        } // end leaf node traversal

        // internal node traversal
        else {
            const std::size_t splitAxis = currentNode.splitAxis();
            const real splitPlaneT 
                = (currentNode.splitPosition() - origin[splitAxis]) * inverseDirection[splitAxis];

            std::size_t nearChildIndex;
            std::size_t farChildIndex;

            // decide which child is near/far node
            // near child is first node
            if ((origin[splitAxis] < currentNode.splitPosition()) ||
                (origin[splitAxis] == currentNode.splitPosition() && direction[splitAxis] <= 0.0_r)) {

                nearChildIndex = currentNodeIndex + 1;
                farChildIndex  = currentNode.secondChildIndex();
            }
            // near child is second node
            else {
                nearChildIndex = currentNode.secondChildIndex();
                farChildIndex  = currentNodeIndex + 1;
            }

            // there are three cases for internal nodes traversal,
            // first two cases don't need to change minT/maxT
            // in currentNodeInfo

            // CaseI: only near child node is hit
            if (currentNodeInfo.maxT() < splitPlaneT || splitPlaneT <= 0.0_r) {
                currentNodeInfo.setNodeIndex(nearChildIndex);
            }
            // CaseII: only far child node is hit
            else if (splitPlaneT < currentNodeInfo.minT()) {
                currentNodeInfo.setNodeIndex(farChildIndex);
            }
            // CaseIII: both children nodes are hit
            else {
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