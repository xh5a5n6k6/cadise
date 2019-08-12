#include "core/intersector/accelerator/bvh/bvhLinearNode.h"

namespace cadise {

BvhLinearNode::BvhLinearNode() :
    _bound() {
}

BvhLinearNode::~BvhLinearNode() = default;

void BvhLinearNode::initializeInternalNode(const AABB3R& bound, const uint64 secondChildIndex, const uint32 splitAxis) {
    _bound = bound;
    _secondChildIndex = secondChildIndex;
    _splitAxis = splitAxis;
    _isLeaf = false;
}

void BvhLinearNode::initializeLeafNode(const AABB3R& bound, const uint64 intersectorIndex, const uint64 intersectorCounts) {
    _bound = bound;
    _intersectorIndex = intersectorIndex;
    _intersectorCounts = intersectorCounts;
    _isLeaf = true;
}

AABB3R BvhLinearNode::bound() const {
    return _bound;
}

uint64 BvhLinearNode::intersectorIndex() const {
    return _intersectorIndex;
}

uint64 BvhLinearNode::secondChildIndex() const {
    return _secondChildIndex;
}

uint64 BvhLinearNode::intersectorCounts() const {
    return _intersectorCounts;
}

uint32 BvhLinearNode::splitAxis() const {
    return _splitAxis;
}

bool BvhLinearNode::isLeaf() const {
    return _isLeaf;
}

} // namespace cadise