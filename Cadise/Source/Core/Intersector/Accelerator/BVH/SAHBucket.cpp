#include "Core/Intersector/Accelerator/BVH/SAHBucket.h"

namespace cadise
{

SAHBucket::SAHBucket() :
    SAHBucket(AABB3R(), 0)
{}

SAHBucket::SAHBucket(const AABB3R& bound, const std::size_t intersectorCount) :
    _bound(bound),
    _intersectorCount(intersectorCount)
{}

void SAHBucket::addOneBoundAndCount(const AABB3R& bound)
{
    _bound.unionWithLocal(bound);
    ++_intersectorCount;
}

const AABB3R& SAHBucket::bound() const
{
    return _bound;
}

std::size_t SAHBucket::intersectorCount() const
{
    return _intersectorCount;
}

} // namespace cadise