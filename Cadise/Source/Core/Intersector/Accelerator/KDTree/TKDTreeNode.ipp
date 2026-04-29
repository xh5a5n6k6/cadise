#pragma once

#include "Core/Intersector/Accelerator/KDTree/TKDTreeNode.h"

#include "Foundation/Assertion.h"

namespace cadise
{

template<typename T>
inline constexpr T MASK = T(3);

template<typename Index>
inline TKDTreeNode<Index>::TKDTreeNode() :
    _objectIndex(0),
    _secondChildIndex(0)
{}

template<typename Index>
inline void TKDTreeNode<Index>::initializInternalNode(
    const std::size_t secondChildIndex,
    const std::size_t splitAxis,
    const real        splitPosition)
{
    _splitPosition     = splitPosition;
    _splitAxis         = static_cast<Index>(splitAxis);
    _secondChildIndex |= static_cast<Index>(secondChildIndex << 2);
}

template<typename Index>
inline void TKDTreeNode<Index>::initializeLeafNode(
    const std::size_t objectIndex,
    const std::size_t numObjects)
{
    _objectIndex  = static_cast<Index>(objectIndex);
    _splitAxis    = MASK<Index>;
    _numObjects  |= static_cast<Index>(numObjects << 2);
}

template<typename Index>
inline std::size_t TKDTreeNode<Index>::objectIndex() const
{
    return static_cast<std::size_t>(_objectIndex);
}

template<typename Index>
inline std::size_t TKDTreeNode<Index>::numObjects() const
{
    return static_cast<std::size_t>(_numObjects >> 2);
}

template<typename Index>
inline real TKDTreeNode<Index>::splitPosition() const
{
    return _splitPosition;
}

template<typename Index>
inline std::size_t TKDTreeNode<Index>::splitAxis() const
{
    return static_cast<std::size_t>(_splitAxis & MASK<Index>);
}

template<typename Index>
inline std::size_t TKDTreeNode<Index>::secondChildIndex() const
{
    return static_cast<std::size_t>(_secondChildIndex >> 2);
}

template<typename Index>
inline bool TKDTreeNode<Index>::isLeaf() const
{
    return (_splitAxis & MASK<Index>) == MASK<Index>;
}

} // namespace cadise