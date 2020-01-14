#include "core/intersector/accelerator/kd-tree/endpoint.h"

namespace cadise {

Endpoint::Endpoint() :
    Endpoint(0, 0.0_r, EndpointType::NONE) {
}

Endpoint::Endpoint(const std::size_t   intersectorIndex,
                   const real          position,
                   const EndpointType& type) :
    _intersectorIndex(intersectorIndex),
    _position(position),
    _type(type) {
}

std::size_t Endpoint::intersectorIndex() const {
    return _intersectorIndex;
}

real Endpoint::position() const {
    return _position;
}

const EndpointType& Endpoint::type() const {
    return _type;
}

} // namespace cadise