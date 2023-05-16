#pragma once

#include "Math/TAABB3.h"

namespace cadise
{

class SAHBucket
{
public:
    SAHBucket();
    SAHBucket(const AABB3R& bound, const std::size_t intersectorCount);

    void addOneBoundAndCount(const AABB3R& bound);

    const AABB3R& bound() const;
    std::size_t intersectorCount() const;

private:
    AABB3R      _bound;
    std::size_t _intersectorCount;
};

} // namespace cadise