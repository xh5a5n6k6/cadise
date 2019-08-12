#include "core/intersector/accelerator/bvh/bvhBinaryNode.h"

namespace cadise {

BVHBinaryNode::BVHBinaryNode() :
    _bound(), 
    _children{} {
}

BVHBinaryNode::~BVHBinaryNode() {
    _children[0].release();
    _children[1].release();
}

void BVHBinaryNode::initializeInternalNode(std::unique_ptr<BVHBinaryNode> firstChild, std::unique_ptr<BVHBinaryNode> secondChild, const uint32 splitAxis) {
    _bound = firstChild->bound().unionWith(secondChild->bound());
    _children[0] = std::move(firstChild);
    _children[1] = std::move(secondChild);
    _splitAxis = splitAxis;
    _isLeaf = false;
}

void BVHBinaryNode::initializeLeafNode(const AABB3R& bound, const uint64 intersectorIndex, const uint64 intersectorCounts) {
    _bound = bound;
    _intersectorIndex = intersectorIndex;
    _intersectorCounts = intersectorCounts;
    _isLeaf = true;
}

AABB3R BVHBinaryNode::bound() const {
    return _bound;
}

uint64 BVHBinaryNode::intersectorIndex() const {
    return _intersectorIndex;
}
std::unique_ptr<BVHBinaryNode> BVHBinaryNode::firstChild() {
    return std::move(_children[0]);
}

std::unique_ptr<BVHBinaryNode> BVHBinaryNode::secondChild() {
    return std::move(_children[1]);
}

uint64 BVHBinaryNode::intersectorCounts() const {
    return _intersectorCounts;
}

uint32 BVHBinaryNode::splitAxis() const {
    return _splitAxis;
}

bool BVHBinaryNode::isLeaf() const {
    return _isLeaf;
}

} // namespace cadise