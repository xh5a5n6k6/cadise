#include "core/intersector/accelerator/bvh/bvhAccelerator.h"

#include "core/intersector/accelerator/bvh/bvhBinaryNode.h"
#include "core/intersector/accelerator/bvh/bvhBuilder.h"

#include "core/ray.h"

namespace cadise {

BVHAccelerator::BVHAccelerator(const std::vector<std::shared_ptr<Intersector> > intersectors) {
    if (intersectors.empty()) {
        exit(1);
    }

    _intersectors.reserve(intersectors.size());

    // select spiltter
    BVHSplitter splitter = BVHSplitter::EQUAL;

    BVHBuilder builder = BVHBuilder(splitter);

    uint64 totalSize = 0;
    // build binary node tree recursively
    std::unique_ptr<BVHBinaryNode> root = builder.buildBinaryNodes(std::move(intersectors), _intersectors, totalSize);

    // flatten the binary tree and use BVHLinearNode to store the informations
    _nodes.clear();
    _nodes.shrink_to_fit();
    builder.buildLinearNodes(std::move(root), _nodes, totalSize);
}

AABB3R BVHAccelerator::bound() const {
    return _nodes[0].bound();
}

bool BVHAccelerator::isIntersecting(Ray &ray, Intersection &intersection) const {
    bool result = false;

    Vector3R origin = ray.origin();
    Vector3R invDirection = ray.direction().reciprocal();
    int32 dirIsNegative[3] = { invDirection.x() < 0.0_r, invDirection.y() < 0.0_r, invDirection.z() < 0.0_r };
    uint64 currentNodeIndex = 0;
    uint64 currentStackSize = 0;
    uint64 nodeStack[MAX_STACK_SIZE];

    while (true) {
        BVHLinearNode currentNode = _nodes[currentNodeIndex];
        if (currentNode.bound().isIntersectingAABB(origin, invDirection, ray.minT(), ray.maxT())) {
            if (currentNode.isLeaf()) {
                for (uint64 i = 0; i < currentNode.intersectorCounts(); i++) {
                    result |= _intersectors[currentNode.intersectorIndex() + i]->isIntersecting(ray, intersection);
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

bool BVHAccelerator::isOccluded(Ray &ray) const {
    bool result = false;

    Vector3R origin = ray.origin();
    Vector3R invDirection = ray.direction().reciprocal();
    int32 dirIsNegative[3] = { invDirection.x() < 0.0_r, invDirection.y() < 0.0_r, invDirection.z() < 0.0_r };
    uint64 currentNodeIndex = 0;
    uint64 currentStackSize = 0;
    uint64 nodeStack[MAX_STACK_SIZE];

    while (true) {
        BVHLinearNode currentNode = _nodes[currentNodeIndex];
        if (currentNode.bound().isIntersectingAABB(origin, invDirection, ray.minT(), ray.maxT())) {
            if (currentNode.isLeaf()) {
                for (uint64 i = 0; i < currentNode.intersectorCounts(); i++) {
                    result |= _intersectors[currentNode.intersectorIndex() + i]->isOccluded(ray);
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

} // namespace cadise