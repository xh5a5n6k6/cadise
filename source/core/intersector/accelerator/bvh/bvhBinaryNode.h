#pragma once

#include "math/aabb.h"

#include <memory>

namespace cadise {

class BvhBinaryNode {
public:
    BvhBinaryNode();
    ~BvhBinaryNode();

    void initializeInternalNode(const AABB3R&                  bound,
                                std::unique_ptr<BvhBinaryNode> firstChild, 
                                std::unique_ptr<BvhBinaryNode> secondChild, 
                                const std::size_t              splitAxis);

    void initializeLeafNode(const AABB3R&     bound, 
                            const std::size_t intersectorIndex, 
                            const std::size_t intersectorCounts);

    // for leaf node
    std::size_t intersectorIndex() const;
    std::size_t intersectorCounts() const;

    // for internal node
    std::unique_ptr<BvhBinaryNode> firstChild();
    std::unique_ptr<BvhBinaryNode> secondChild();
    std::size_t splitAxis() const;

    const AABB3R& bound() const;
    bool isLeaf() const;

private:
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
        std::size_t _splitAxis;
    };
    
    AABB3R _bound;
    bool   _isLeaf;
};

} // namespace cadise