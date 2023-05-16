#pragma once

#include "Core/Intersector/Accelerator/KDTree/EEndpointType.h"
#include "Math/Type/PrimitiveType.h"

namespace cadise
{

class Endpoint
{
public:
    Endpoint();
    Endpoint(
        const std::size_t   intersectorIndex,
        const real          position,
        const EEndpointType type);

    std::size_t intersectorIndex() const;
    real position() const;
    EEndpointType type() const;

private:
    std::size_t   _intersectorIndex;
    real          _position;
    EEndpointType _type;
};

} // namespace cadise