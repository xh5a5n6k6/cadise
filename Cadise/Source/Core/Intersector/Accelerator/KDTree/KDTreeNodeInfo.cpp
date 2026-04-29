#include "Core/Intersector/Accelerator/KDTree/KDTreeNodeInfo.h"

namespace cadise {

KDTreeNodeInfo::KDTreeNodeInfo() :
    KDTreeNodeInfo(0, 0.0_r, 0.0_r)
{}

KDTreeNodeInfo::KDTreeNodeInfo(
    const std::size_t nodeIndex,
    const real        minT,
    const real        maxT) :

    _nodeIndex(nodeIndex),
    _minT(minT),
    _maxT(maxT)
{}

std::size_t KDTreeNodeInfo::nodeIndex() const
{
    return _nodeIndex;
}

real KDTreeNodeInfo::minT() const
{
    return _minT;
}

real KDTreeNodeInfo::maxT() const
{
    return _maxT;
}

void KDTreeNodeInfo::setNodeIndex(const std::size_t nodeIndex)
{
    _nodeIndex = nodeIndex;
}

void KDTreeNodeInfo::setMinT(const real minT)
{
    _minT = minT;
}

void KDTreeNodeInfo::setMaxT(const real maxT)
{
    _maxT = maxT;
}

} // namespace cadise