#pragma once

#include "core/intersector/accelerator/kd-tree/kdTreeNode.h"

#include "fundamental/assertion.h"

namespace cadise {

template<typename T>
inline constexpr T MASK = T(3);

template<typename Index>
KdTreeNode<Index>::KdTreeNode() = default;

template<typename Index>
void KdTreeNode<Index>::initializInternalNode(
    const std::size_t secondChildIndex,
    const std::size_t splitAxis,
    const real        splitPosition) {

    _splitPosition     = splitPosition;
    _splitAxis         = static_cast<Index>(splitAxis);
    _secondChildIndex |= static_cast<Index>(secondChildIndex << 2);
}

template<typename Index>
void KdTreeNode<Index>::initializeLeafNode(
    const std::size_t objectIndex,
    const std::size_t numObjects) {

    _objectIndex  = static_cast<Index>(objectIndex);
    _splitAxis    = MASK<Index>;
    _numObjects  |= static_cast<Index>(numObjects << 2);
}

template<typename Index>
std::size_t KdTreeNode<Index>::objectIndex() const {
    return static_cast<std::size_t>(_objectIndex);
}

template<typename Index>
std::size_t KdTreeNode<Index>::numObjects() const {
    return static_cast<std::size_t>(_numObjects >> 2);
}

template<typename Index>
real KdTreeNode<Index>::splitPosition() const {
    return _splitPosition;
}

template<typename Index>
std::size_t KdTreeNode<Index>::splitAxis() const {
    return static_cast<std::size_t>(_splitAxis & MASK<Index>);
}

template<typename Index>
std::size_t KdTreeNode<Index>::secondChildIndex() const {
    return static_cast<std::size_t>(_secondChildIndex >> 2);
}

template<typename Index>
bool KdTreeNode<Index>::isLeaf() const {
    return (_splitAxis & MASK<Index>) == MASK<Index>;
}

} // namespace cadise