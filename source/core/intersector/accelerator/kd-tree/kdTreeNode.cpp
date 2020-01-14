#include "core/intersector/accelerator/kd-tree/kdTreeNode.h"

namespace cadise {

static const std::size_t MASK = 3;

KdTreeNode::KdTreeNode() = default;

void KdTreeNode::initializInternalNode(const std::size_t secondChildIndex,
                                       const std::size_t splitAxis,
                                       const real        splitPosition) {

    _splitPosition     = splitPosition;
    _splitAxis         = splitAxis;
    _secondChildIndex |= (secondChildIndex << 2);
}

void KdTreeNode::initializeLeafNode(const std::size_t intersectorIndex,
                                    const std::size_t intersectorCounts) {

    _intersectorIndex   = intersectorIndex;
    _splitAxis          = MASK;
    _intersectorCounts |= (intersectorCounts << 2);
}

std::size_t KdTreeNode::intersectorIndex() const {
    return _intersectorIndex;
}

std::size_t KdTreeNode::intersectorCounts() const {
    return _intersectorCounts >> 2;
}

real KdTreeNode::splitPosition() const {
    return _splitPosition;
}

std::size_t KdTreeNode::splitAxis() const {
    return _splitAxis & MASK;
}

std::size_t KdTreeNode::secondChildIndex() const {
    return _secondChildIndex >> 2;
}

bool KdTreeNode::isLeaf() const {
    return (_splitAxis & MASK) == MASK;
}

} // namespace cadise