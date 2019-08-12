#include "core/intersector/accelerator/bvh/bvhAccelerator.h"

#include "core/intersector/accelerator/bvh/bvhBinaryNode.h"
#include "core/intersector/accelerator/bvh/bvhBuilder.h"

#include "core/ray.h"

namespace cadise {

BvhAccelerator::BvhAccelerator(const std::vector<std::shared_ptr<Intersector>>& intersectors) {
    if (intersectors.empty()) {
        exit(1);
    }

    _intersectors.reserve(intersectors.size());

    // select spiltter
    BvhSplitter splitter = BvhSplitter::EQUAL;

    BvhBuilder builder = BvhBuilder(splitter);

    uint64 totalSize = 0;
    // build binary node tree recursively
    std::unique_ptr<BvhBinaryNode> root = builder.buildBinaryNodes(std::move(intersectors), _intersectors, totalSize);

    // flatten the binary tree and use BVHLinearNode to store the informations
    _nodes.clear();
    _nodes.shrink_to_fit();
    builder.buildLinearNodes(std::move(root), _nodes, totalSize);
}

AABB3R BvhAccelerator::bound() const {
    return _nodes[0].bound();
}

bool BvhAccelerator::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const {
    bool result = false;

    Vector3R origin = ray.origin();
    Vector3R invDirection = ray.direction().reciprocal();
    int32 dirIsNegative[3] = { invDirection.x() < 0.0_r, invDirection.y() < 0.0_r, invDirection.z() < 0.0_r };
    uint64 currentNodeIndex = 0;
    uint64 currentStackSize = 0;
    uint64 nodeStack[MAX_STACK_SIZE];

    while (true) {
        BvhLinearNode currentNode = _nodes[currentNodeIndex];
        if (currentNode.bound().isIntersectingAABB(origin, invDirection, ray.minT(), ray.maxT())) {
            if (currentNode.isLeaf()) {
                for (uint64 i = 0; i < currentNode.intersectorCounts(); i++) {
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
                if (dirIsNegative[currentNode.splitAxis()]) {
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

bool BvhAccelerator::isOccluded(Ray& ray) const {
    Vector3R origin = ray.origin();
    Vector3R invDirection = ray.direction().reciprocal();
    int32 dirIsNegative[3] = { invDirection.x() < 0.0_r, invDirection.y() < 0.0_r, invDirection.z() < 0.0_r };
    uint64 currentNodeIndex = 0;
    uint64 currentStackSize = 0;
    uint64 nodeStack[MAX_STACK_SIZE];

    while (true) {
        BvhLinearNode currentNode = _nodes[currentNodeIndex];
        if (currentNode.bound().isIntersectingAABB(origin, invDirection, ray.minT(), ray.maxT())) {
            if (currentNode.isLeaf()) {
                for (uint64 i = 0; i < currentNode.intersectorCounts(); i++) {
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
                if (dirIsNegative[currentNode.splitAxis()]) {
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