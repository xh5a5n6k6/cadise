#include "core/intersector/accelerator/bvh/bvhBoundInfo.h"

namespace cadise {

BvhBoundInfo::BvhBoundInfo() :
    BvhBoundInfo(AABB3R(), 0) {
}

BvhBoundInfo::BvhBoundInfo(const AABB3R& bound, const std::size_t index) :
    _bound(bound),
    _index(index) {

    _centroid = bound.centroid();
}

const AABB3R& BvhBoundInfo::bound() const {
    return _bound;
}

const Vector3R& BvhBoundInfo::centroid() const {
    return _centroid;
}

std::size_t BvhBoundInfo::index() const {
    return _index;
}

} // namespace cadise