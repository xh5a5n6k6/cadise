#pragma once

#include "Math/Type/PrimitiveType.h"

namespace cadise 
{

class KDTreeNodeInfo 
{
public:
    KDTreeNodeInfo();
    KDTreeNodeInfo(
        const std::size_t nodeIndex,
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