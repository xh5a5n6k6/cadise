#pragma once

#include "math/tAabb3.h"

namespace cadise {

class BvhSahBucket {
public:
    BvhSahBucket();
    BvhSahBucket(const AABB3R& bound, const std::size_t intersectorCounts);

    void addOneBoundAndCount(const AABB3R& bound);

    const AABB3R& bound() const;
    std::size_t intersectorCounts() const;

private:
    AABB3R      _bound;
    std::size_t _intersectorCounts;
};

} // namespace cadise