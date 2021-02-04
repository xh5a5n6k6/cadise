#include "core/renderer/photon-mapping/photon.h"

namespace cadise {

Photon::Photon() = default;

const Vector3R& Photon::position() const {
    return _position;
}

const Vector3R& Photon::fromDirection() const {
    return _fromDirection;
}

const Spectrum& Photon::throughputRadiance() const {
    return _throughputRadiance;
}

void Photon::setPosition(const Vector3R& position) {
    _position = position;
}

void Photon::setFromDirection(const Vector3R& fromDirection) {
    _fromDirection = fromDirection;
}

void Photon::setThroughputRadiance(const Spectrum& throughputRadiance) {
    _throughputRadiance = throughputRadiance;
}

} // namespace cadise