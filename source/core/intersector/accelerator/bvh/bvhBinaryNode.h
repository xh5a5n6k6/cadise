#pragma once

#include "math/aabb.h"

#include <memory>

namespace cadise {

class BvhBinaryNode {
public:
    BvhBinaryNode();
    ~BvhBinaryNode();

    void initializeInternalNode(std::unique_ptr<BvhBinaryNode> firstChild, std::unique_ptr<BvhBinaryNode> secondChild, const uint32 splitAxis);
    void initializeLeafNode(const AABB3R& bound, const uint64 intersectorIndex, const uint64 intersectorCounts);

    AABB3R bound() const;
    uint64 intersectorIndex() const;
    std::unique_ptr<BvhBinaryNode> firstChild();
    std::unique_ptr<BvhBinaryNode> secondChild();
    uint64 intersectorCounts() const;
    uint32 splitAxis() const;
    bool isLeaf() const;

private:
    AABB3R _bound;

    union {
        // for leaf node
        uint64 _intersectorIndex;
        // for internal node
        std::unique_ptr<BvhBinaryNode> _children[2];
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