#include "core/intersector/accelerator/bvh/bvhLinearNode.h"

namespace cadise {

BvhLinearNode::BvhLinearNode() :
    _bound() {
}

BvhLinearNode::~BvhLinearNode() = default;

void BvhLinearNode::initializeInternalNode(const AABB3R&     bound, 
                                           const std::size_t secondChildIndex, 
                                           const std::size_t splitAxis) {

    _bound            = bound;
    _secondChildIndex = secondChildIndex;
    _splitAxis        = splitAxis;
    _isLeaf           = false;
}

void BvhLinearNode::initializeLeafNode(const AABB3R&     bound, 
                                       const std::size_t intersectorIndex, 
                                       const std::size_t intersectorCounts) {

    _bound             = bound;
    _intersectorIndex  = intersectorIndex;
    _intersectorCounts = intersectorCounts;
    _isLeaf            = true;
}

std::size_t BvhLinearNode::intersectorIndex() const {
    return _intersectorIndex;
}

std::size_t BvhLinearNode::intersectorCounts() const {
    return _intersectorCounts;
}

std::size_t BvhLinearNode::secondChildIndex() const {
    return _secondChildIndex;
}

std::size_t BvhLinearNode::splitAxis() const {
    return _splitAxis;
}

const AABB3R& BvhLinearNode::bound() const {
    return _bound;
}

bool BvhLinearNode::isLeaf() const {
    return _isLeaf;
}

} // namespace cadise