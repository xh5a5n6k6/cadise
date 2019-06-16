#pragma once

#include "math/aabb.h"

namespace cadise {

class BVHLinearNode {
public:
    BVHLinearNode();
    ~BVHLinearNode();

    void initializeInternalNode(const AABB3R bound, const uint64 secondChildIndex, const uint32 splitAxis);
    void initializeLeafNode(const AABB3R bound, const uint64 intersectorIndex, const uint64 intersectorCounts);

    AABB3R bound() const;
    uint64 intersectorIndex() const;
    uint64 secondChildIndex() const;
    uint64 intersectorCounts() const;
    uint32 splitAxis() const;
    bool isLeaf() const;

private:
    AABB3R _bound;

    union {
        // for leaf node
        uint64 _intersectorIndex;
        // for internal node
        uint64 _secondChildIndex;
    };

    union {
        // for leaf node
        uint64 _intersectorCounts;
        // for internal node
        uint32 _splitAxis;
    };

    bool _isLeaf;
};

} // namespace cadise