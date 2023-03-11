#include "Core/Intersector/Accelerator/BVH/BVHBinaryNode.h"

namespace cadise 
{

BVHBinaryNode::BVHBinaryNode() :
    _children{},
    _bound()
{}

BVHBinaryNode::~BVHBinaryNode()
{
    _children[0].release();
    _children[1].release();
}

void BVHBinaryNode::initializeInternalNode(
    const AABB3R&                  bound,
    std::unique_ptr<BVHBinaryNode> firstChild, 
    std::unique_ptr<BVHBinaryNode> secondChild, 
    const std::size_t              splitAxis) 
{
    _bound       = bound;
    _children[0] = std::move(firstChild);
    _children[1] = std::move(secondChild);
    _splitAxis   = splitAxis;
    _isLeaf      = false;
}

void BVHBinaryNode::initializeLeafNode(
    const AABB3R&     bound, 
    const std::size_t intersectorIndex, 
    const std::size_t intersectorCounts)
{
    _bound             = bound;
    _intersectorIndex  = intersectorIndex;
    _intersectorCounts = intersectorCounts;
    _isLeaf            = true;
}

std::size_t BVHBinaryNode::intersectorIndex() const 
{
    return _intersectorIndex;
}

std::size_t BVHBinaryNode::intersectorCounts() const 
{
    return _intersectorCounts;
}

std::unique_ptr<BVHBinaryNode> BVHBinaryNode::firstChild() 
{
    return std::move(_children[0]);
}

std::unique_ptr<BVHBinaryNode> BVHBinaryNode::secondChild() 
{
    return std::move(_children[1]);
}

std::size_t BVHBinaryNode::splitAxis() const 
{
    return _splitAxis;
}

const AABB3R& BVHBinaryNode::bound() const 
{
    return _bound;
}

bool BVHBinaryNode::isLeaf() const
{
    return _isLeaf;
}

} // namespace cadise