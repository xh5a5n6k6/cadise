#pragma once

#include "math/type/fundamentalType.h"

namespace cadise 
{

class KdTreeNodeInfo 
{
public:
    KdTreeNodeInfo();
    KdTreeNodeInfo(const std::size_t nodeIndex,
                   const real        minT,
                   const real        maxT);

    std::size_t nodeIndex() const;
    real minT() const;
    real maxT() const;

    void setNodeIndex(const std::size_t nodeIndex);
    void setMinT(const real minT);
    void setMaxT(const real maxT);

private:
    std::size_t _nodeIndex;
    real        _minT;
    real        _maxT;
};

} // namespace cadise