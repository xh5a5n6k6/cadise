#include "core/intersector/accelerator/kd-tree/kdTreeNodeInfo.h"

namespace cadise {

KdTreeNodeInfo::KdTreeNodeInfo() :
    KdTreeNodeInfo(0, 0.0_r, 0.0_r) {

}

KdTreeNodeInfo::KdTreeNodeInfo(const std::size_t nodeIndex,
                               const real        minT,
                               const real        maxT) :
    _nodeIndex(nodeIndex),
    _minT(minT),
    _maxT(maxT) {
}

std::size_t KdTreeNodeInfo::nodeIndex() const {
    return _nodeIndex;
}

real KdTreeNodeInfo::minT() const {
    return _minT;
}

real KdTreeNodeInfo::maxT() const {
    return _maxT;
}

void KdTreeNodeInfo::setNodeIndex(const std::size_t nodeIndex) {
    _nodeIndex = nodeIndex;
}

void KdTreeNodeInfo::setMinT(const real minT) {
    _minT = minT;
}

void KdTreeNodeInfo::setMaxT(const real maxT) {
    _maxT = maxT;
}

} // namespace cadise