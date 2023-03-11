#include "Core/Intersector/Accelerator/BVH/BVHBoundInfo.h"

namespace cadise 
{

BVHBoundInfo::BVHBoundInfo() :
    BVHBoundInfo(AABB3R(), 0) 
{}

BVHBoundInfo::BVHBoundInfo(const AABB3R& bound, const std::size_t index) :
    _bound(bound),
    _index(index)
{
    _centroid = bound.centroid();
}

const AABB3R& BVHBoundInfo::bound() const
{
    return _bound;
}

const Vector3R& BVHBoundInfo::centroid() const 
{
    return _centroid;
}

std::size_t BVHBoundInfo::index() const 
{
    return _index;
}

} // namespace cadise