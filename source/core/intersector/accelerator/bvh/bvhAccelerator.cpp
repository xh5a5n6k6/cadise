#include "core/intersector/accelerator/bvh/bvhAccelerator.h"

#include "core/intersector/accelerator/bvh/bvhBinaryNode.h"
#include "core/intersector/accelerator/bvh/bvhBuilder.h"
#include "core/ray.h"

namespace cadise {

BvhAccelerator::BvhAccelerator(const std::vector<std::shared_ptr<Intersector>>& intersectors) {
    _intersectors.reserve(intersectors.size());

    // select spiltter
    const BvhSplitMode splitMode = BvhSplitMode::EQUAL;

    const BvhBuilder builder(splitMode);

    std::size_t totalSize = 0;
    // build binary node tree recursively
    std::unique_ptr<BvhBinaryNode> root = builder.buildBinaryNodes(std::move(intersectors), 
                                                                   &_intersectors, 
                                                                   &totalSize);

    // flatten the binary tree and use BVHLinearNode to store the informations
    _nodes.clear();
    _nodes.shrink_to_fit();
    builder.buildLinearNodes(std::move(root), totalSize, &_nodes);
}

AABB3R BvhAccelerator::bound() const {
    return _nodes[0].bound();
}

bool BvhAccelerator::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const {
    bool result = false;

    const Vector3R origin           = ray.origin();
    const Vector3R inverseDirection = ray.direction().reciprocal();
    const int32 directionIsNegative[3] = { inverseDirection.x() < 0.0_r, 
                                           inverseDirection.y() < 0.0_r, 
                                           inverseDirection.z() < 0.0_r };
    
    std::size_t currentNodeIndex = 0;
    std::size_t currentStackSize = 0;
    std::size_t nodeStack[MAX_STACK_SIZE];

    while (true) {
        const BvhLinearNode currentNode = _nodes[currentNodeIndex];

        if (currentNode.bound().isIntersectingAABB(origin, inverseDirection, ray.minT(), ray.maxT())) {
            if (currentNode.isLeaf()) {
                for (std::size_t i = 0; i < currentNode.intersectorCounts(); ++i) {
                    result |= _intersectors[currentNode.intersectorIndex() + i]->isIntersecting(ray, primitiveInfo);
                }

                if (currentStackSize == 0) {
                    break;
                }
                else {
                    --currentStackSize;
                    currentNodeIndex = nodeStack[currentStackSize];
                }
            } // end leaf node

            else {
                if (directionIsNegative[currentNode.splitAxis()]) {
                    nodeStack[currentStackSize] = currentNodeIndex + 1;
                    ++currentStackSize;
                    currentNodeIndex = currentNode.secondChildIndex();
                }
                else {
                    nodeStack[currentStackSize] = currentNode.secondChildIndex();
                    ++currentStackSize;
                    currentNodeIndex = currentNodeIndex + 1;
                }
            } // end internal node
        } // end node intersection

        else {
            if (currentStackSize == 0) {
                break;
            }
            else {
                --currentStackSize;
                currentNodeIndex = nodeStack[currentStackSize];
            }
        }
    } // end while loop

    return result;
}

bool BvhAccelerator::isOccluded(const Ray& ray) const {
    const Vector3R origin           = ray.origin();
    const Vector3R inverseDirection = ray.direction().reciprocal();
    const int32 directionIsNegative[3] = { inverseDirection.x() < 0.0_r,
                                           inverseDirection.y() < 0.0_r,
                                           inverseDirection.z() < 0.0_r };

    std::size_t currentNodeIndex = 0;
    std::size_t currentStackSize = 0;
    std::size_t nodeStack[MAX_STACK_SIZE];

    while (true) {
        const BvhLinearNode currentNode = _nodes[currentNodeIndex];

        if (currentNode.bound().isIntersectingAABB(origin, inverseDirection, ray.minT(), ray.maxT())) {
            if (currentNode.isLeaf()) {
                for (std::size_t i = 0; i < currentNode.intersectorCounts(); ++i) {
                    if (_intersectors[currentNode.intersectorIndex() + i]->isOccluded(ray)) {
                        return true;
                    }
                }

                if (currentStackSize == 0) {
                    break;
                }
                else {
                    --currentStackSize;
                    currentNodeIndex = nodeStack[currentStackSize];
                }
            } // end leaf node

            else {
                if (directionIsNegative[currentNode.splitAxis()]) {
                    nodeStack[currentStackSize] = currentNodeIndex + 1;
                    ++currentStackSize;
                    currentNodeIndex = currentNode.secondChildIndex();
                }
                else {
                    nodeStack[currentStackSize] = currentNode.secondChildIndex();
                    ++currentStackSize;
                    currentNodeIndex = currentNodeIndex + 1;
                }
            } // end internal node
        } // end node intersection

        else {
            if (currentStackSize == 0) {
                break;
            }
            else {
                --currentStackSize;
                currentNodeIndex = nodeStack[currentStackSize];
            }
        }
    } // end while loop

    return false;
}

} // namespace cadise