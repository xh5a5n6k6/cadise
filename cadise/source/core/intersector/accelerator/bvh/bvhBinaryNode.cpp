#include "core/intersector/accelerator/bvh/bvhBinaryNode.h"

namespace cadise {

BvhBinaryNode::BvhBinaryNode() :
    _children{},
    _bound() {
}

BvhBinaryNode::~BvhBinaryNode() {
    _children[0].release();
    _children[1].release();
}

void BvhBinaryNode::initializeInternalNode(
    const AABB3R&                  bound,
    std::unique_ptr<BvhBinaryNode> firstChild, 
    std::unique_ptr<BvhBinaryNode> secondChild, 
    const std::size_t              splitAxis) {

    _bound       = bound;
    _children[0] = std::move(firstChild);
    _children[1] = std::move(secondChild);
    _splitAxis   = splitAxis;
    _isLeaf      = false;
}

void BvhBinaryNode::initializeLeafNode(
    const AABB3R&     bound, 
    const std::size_t intersectorIndex, 
    const std::size_t intersectorCounts) {

    _bound             = bound;
    _intersectorIndex  = intersectorIndex;
    _intersectorCounts = intersectorCounts;
    _isLeaf            = true;
}

std::size_t BvhBinaryNode::intersectorIndex() const {
    return _intersectorIndex;
}

std::size_t BvhBinaryNode::intersectorCounts() const {
    return _intersectorCounts;
}

std::unique_ptr<BvhBinaryNode> BvhBinaryNode::firstChild() {
    return std::move(_children[0]);
}

std::unique_ptr<BvhBinaryNode> BvhBinaryNode::secondChild() {
    return std::move(_children[1]);
}

std::size_t BvhBinaryNode::splitAxis() const {
    return _splitAxis;
}

const AABB3R& BvhBinaryNode::bound() const {
    return _bound;
}

bool BvhBinaryNode::isLeaf() const {
    return _isLeaf;
}

} // namespace cadise