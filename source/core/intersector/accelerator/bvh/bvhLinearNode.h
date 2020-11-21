#pragma once

#include "math/tAabb.h"

namespace cadise {

class BvhLinearNode {
public:
    BvhLinearNode();
    ~BvhLinearNode();

    void initializeInternalNode(const AABB3R&     bound, 
                                const std::size_t secondChildIndex, 
                                const std::size_t splitAxis);

    void initializeLeafNode(const AABB3R&     bound, 
                            const std::size_t intersectorIndex, 
                            const std::size_t intersectorCounts);
    
    // for leaf node
    std::size_t intersectorIndex() const;
    std::size_t intersectorCounts() const;

    // for internal node
    std::size_t secondChildIndex() const;
    std::size_t splitAxis() const;

    const AABB3R& bound() const;
    bool isLeaf() const;

private:
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
        std::size_t _splitAxis;
    };

    AABB3R _bound;
    bool   _isLeaf;
};

} // namespace cadise