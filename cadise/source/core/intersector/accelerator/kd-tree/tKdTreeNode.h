#pragma once

#include "math/type/fundamentalType.h"

namespace cadise 
{

template<typename Index = std::size_t>
class TKdTreeNode
{
public:
    TKdTreeNode();

    void initializInternalNode(
        const std::size_t secondChildIndex,
        const std::size_t splitAxis,
        const real        splitPosition);

    void initializeLeafNode(
        const std::size_t objectIndex,
        const std::size_t numObjects);

    // for leaf node
    std::size_t objectIndex() const;
    std::size_t numObjects() const;

    // for internal node
    real splitPosition() const;
    std::size_t splitAxis() const;
    std::size_t secondChildIndex() const;

    bool isLeaf() const;

private:
    union
    {
        // for leaf node
        Index _objectIndex;
        // for internal node
        real  _splitPosition;
    };

    union
    {
        /*
            use last two bits to recognize if it is leaf node,
            remaining bits store object count in leaf node
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
        Index _splitAxis;
        // for leaf node
        Index _numObjects;
        // for internal node
        Index _secondChildIndex;
    };
};

} // namespace cadise

#include "core/intersector/accelerator/kd-tree/tKdTreeNode.ipp"