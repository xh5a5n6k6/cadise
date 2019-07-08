#include "core/intersector/accelerator/bvh/bvhLinearNode.h"

namespace cadise {

BVHLinearNode::BVHLinearNode() :
    _bound() {
}

BVHLinearNode::~BVHLinearNode() = default;

void BVHLinearNode::initializeInternalNode(const AABB3R& bound, const uint64 secondChildIndex, const uint32 splitAxis) {
    _bound = bound;
    _secondChildIndex = secondChildIndex;
    _splitAxis = splitAxis;
    _isLeaf = false;
}

void BVHLinearNode::initializeLeafNode(const AABB3R& bound, const uint64 intersectorIndex, const uint64 intersectorCounts) {
    _bound = bound;
    _intersectorIndex = intersectorIndex;
    _intersectorCounts = intersectorCounts;
    _isLeaf = true;
}

AABB3R BVHLinearNode::bound() const {
    return _bound;
}

uint64 BVHLinearNode::intersectorIndex() const {
    return _intersectorIndex;
}

uint64 BVHLinearNode::secondChildIndex() const {
    return _secondChildIndex;
}

uint64 BVHLinearNode::intersectorCounts() const {
    return _intersectorCounts;
}

uint32 BVHLinearNode::splitAxis() const {
    return _splitAxis;
}

bool BVHLinearNode::isLeaf() const {
    return _isLeaf;
}

} // namespace cadise