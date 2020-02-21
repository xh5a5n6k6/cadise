#pragma once

#include "math/type/fundamentalType.h"

namespace cadise {

class KdTreeNode {
public:
    KdTreeNode();

    void initializInternalNode(const std::size_t secondChildIndex,
                               const std::size_t splitAxis,
                               const real        splitPosition);

    void initializeLeafNode(const std::size_t intersectorIndex,
                            const std::size_t intersectorCounts);

    // for leaf node
    std::size_t intersectorIndex() const;
    std::size_t intersectorCounts() const;

    // for internal node
    real splitPosition() const;
    std::size_t splitAxis() const;
    std::size_t secondChildIndex() const;

    bool isLeaf() const;

private:
    union {
        // for leaf node
        std::size_t _intersectorIndex;
        // for internal node
        real _splitPosition;
    };

    union {
        /*
            use last two bits to recognize if it is leaf node,
            remaining bits store intersector count in leaf node
            or second child index in internal node.
        
            Last two bits represent as follows:
            00: for x-axis
            01: for y-axis
            10: for z-axis
            11: for leaf node

            Reference:
            PBRT-v3 src/accelerators/kdtreeaccel.cpp
        */
        
        // for both nodes
        std::size_t _splitAxis;
        // for leaf node
        std::size_t _intersectorCounts;
        // for internal node
        std::size_t _secondChildIndex;
    };
};

} // namespace cadise