#include "core/intersector/accelerator/bvh/bvhAccelerator.h"

#include "core/intersector/accelerator/bvh/bvhBinaryNode.h"
#include "core/intersector/accelerator/bvh/bvhBuilder.h"

#include "core/ray.h"

namespace cadise {

BvhAccelerator::BvhAccelerator(const std::vector<std::shared_ptr<Intersector>>& intersectors) {
    // TODO: just create a empty node tree
    if (intersectors.empty()) {
        exit(1);
    }

    _intersectors.reserve(intersectors.size());

    // select spiltter
    const BvhSplitter splitter = BvhSplitter::EQUAL;

    const BvhBuilder builder(splitter);

    std::size_t totalSize = 0;
    // build binary node tree recursively
    std::unique_ptr<BvhBinaryNode> root = builder.buildBinaryNodes(std::move(intersectors), &_intersectors, &totalSize);

    // flatten the binary tree and use BVHLinearNode to store the informations
    _nodes.clear();
    _nodes.shrink_to_fit();
    builder.buildLinearNodes(std::move(root), &_nodes, totalSize);
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
                    currentStackSize -= 1;
                    currentNodeIndex = nodeStack[currentStackSize];
                }
            }
            else {
                if (directionIsNegative[currentNode.splitAxis()]) {
                    nodeStack[currentStackSize] = currentNodeIndex + 1;
                    currentStackSize += 1;
                    currentNodeIndex = currentNode.secondChildIndex();
                }
                else {
                    nodeStack[currentStackSize] = currentNode.secondChildIndex();
                    currentStackSize += 1;
                    currentNodeIndex = currentNodeIndex + 1;
                }
            }
        }
        else {
            if (currentStackSize == 0) {
                break;
            }
            else {
                currentStackSize -= 1;
                currentNodeIndex = nodeStack[currentStackSize];
            }
        }
    }

    return result;
}

bool BvhAccelerator::isOccluded(const Ray& ray) const {
    bool result = false;

    const Vector3R origin = ray.origin();
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
                    currentStackSize -= 1;
                    currentNodeIndex = nodeStack[currentStackSize];
                }
            }
            else {
                if (directionIsNegative[currentNode.splitAxis()]) {
                    nodeStack[currentStackSize] = currentNodeIndex + 1;
                    currentStackSize += 1;
                    currentNodeIndex = currentNode.secondChildIndex();
                }
                else {
                    nodeStack[currentStackSize] = currentNode.secondChildIndex();
                    currentStackSize += 1;
                    currentNodeIndex = currentNodeIndex + 1;
                }
            }
        }
        else {
            if (currentStackSize == 0) {
                break;
            }
            else {
                currentStackSize -= 1;
                currentNodeIndex = nodeStack[currentStackSize];
            }
        }
    }

    return false;
}

} // namespace cadise