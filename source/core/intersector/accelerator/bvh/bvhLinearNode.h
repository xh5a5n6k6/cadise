#pragma once

#include "math/aabb.h"

namespace cadise {

class BvhLinearNode {
public:
    BvhLinearNode();
    ~BvhLinearNode();

    void initializeInternalNode(const AABB3R& bound, const std::size_t secondChildIndex, const uint32 splitAxis);
    void initializeLeafNode(const AABB3R& bound, const std::size_t intersectorIndex, const std::size_t intersectorCounts);

    AABB3R bound() const;
    std::size_t intersectorIndex() const;
    std::size_t secondChildIndex() const;
    std::size_t intersectorCounts() const;
    uint32 splitAxis() const;
    bool isLeaf() const;

private:
    AABB3R _bound;

    union {
        // for leaf node
        std::size_t _intersectorIndex;
        // for internal node
        std::size_t _secondChildIndex;
    };

    union {
        // for leaf node
        std::size_t _intersectorCounts;
        // for internal node
        uint32 _splitAxis;
    };

    bool _isLeaf;
};

} // namespace cadise