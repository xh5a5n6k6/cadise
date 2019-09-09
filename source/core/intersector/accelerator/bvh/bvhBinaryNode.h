#pragma once

#include "math/aabb.h"

#include <memory>

namespace cadise {

class BvhBinaryNode {
public:
    BvhBinaryNode();
    ~BvhBinaryNode();

    void initializeInternalNode(std::unique_ptr<BvhBinaryNode> firstChild, std::unique_ptr<BvhBinaryNode> secondChild, const uint32 splitAxis);
    void initializeLeafNode(const AABB3R& bound, const std::size_t intersectorIndex, const std::size_t intersectorCounts);

    AABB3R bound() const;
    std::size_t intersectorIndex() const;
    std::unique_ptr<BvhBinaryNode> firstChild();
    std::unique_ptr<BvhBinaryNode> secondChild();
    std::size_t intersectorCounts() const;
    uint32 splitAxis() const;
    bool isLeaf() const;

private:
    AABB3R _bound;

    union {
        // for leaf node
        std::size_t _intersectorIndex;
        // for internal node
        std::unique_ptr<BvhBinaryNode> _children[2];
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