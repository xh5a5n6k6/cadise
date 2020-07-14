#pragma once

#include "core/intersector/accelerator/kd-tree/endpointType.h"
#include "math/type/fundamentalType.h"

namespace cadise {

class Endpoint {
public:
    Endpoint();
    Endpoint(
        const std::size_t    intersectorIndex, 
        const real           position,
        const EEndpointType& type);

    std::size_t intersectorIndex() const;
    real position() const;
    const EEndpointType& type() const;

private:
    std::size_t   _intersectorIndex;
    real          _position;
    EEndpointType _type;
};

} // namespace cadise