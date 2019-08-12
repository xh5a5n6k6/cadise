#include "core/intersector/accelerator/bvh/bvhBinaryNode.h"

namespace cadise {

BvhBinaryNode::BvhBinaryNode() :
    _bound(), 
    _children{} {
}

BvhBinaryNode::~BvhBinaryNode() {
    _children[0].release();
    _children[1].release();
}

void BvhBinaryNode::initializeInternalNode(std::unique_ptr<BvhBinaryNode> firstChild, std::unique_ptr<BvhBinaryNode> secondChild, const uint32 splitAxis) {
    _bound = firstChild->bound().unionWith(secondChild->bound());
    _children[0] = std::move(firstChild);
    _children[1] = std::move(secondChild);
    _splitAxis = splitAxis;
    _isLeaf = false;
}

void BvhBinaryNode::initializeLeafNode(const AABB3R& bound, const uint64 intersectorIndex, const uint64 intersectorCounts) {
    _bound = bound;
    _intersectorIndex = intersectorIndex;
    _intersectorCounts = intersectorCounts;
    _isLeaf = true;
}

AABB3R BvhBinaryNode::bound() const {
    return _bound;
}

uint64 BvhBinaryNode::intersectorIndex() const {
    return _intersectorIndex;
}
std::unique_ptr<BvhBinaryNode> BvhBinaryNode::firstChild() {
    return std::move(_children[0]);
}

std::unique_ptr<BvhBinaryNode> BvhBinaryNode::secondChild() {
    return std::move(_children[1]);
}

uint64 BvhBinaryNode::intersectorCounts() const {
    return _intersectorCounts;
}

uint32 BvhBinaryNode::splitAxis() const {
    return _splitAxis;
}

bool BvhBinaryNode::isLeaf() const {
    return _isLeaf;
}

} // namespace cadise