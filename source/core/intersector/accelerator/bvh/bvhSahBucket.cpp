#include "core/intersector/accelerator/bvh/bvhSahBucket.h"

namespace cadise {

BvhSahBucket::BvhSahBucket() :
    BvhSahBucket(AABB3R(), 0) {
}

BvhSahBucket::BvhSahBucket(const AABB3R& bound, const std::size_t intersectorCounts) :
    _bound(bound),
    _intersectorCounts(intersectorCounts) {
}

void BvhSahBucket::addOneBoundAndCount(const AABB3R& bound) {
    _bound.unionWith(bound);
    ++_intersectorCounts;
}

const AABB3R& BvhSahBucket::bound() const {
    return _bound;
}

std::size_t BvhSahBucket::intersectorCounts() const {
    return _intersectorCounts;
}

} // namespace cadise