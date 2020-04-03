#include "core/renderer/pm-renderer/photon.h"

namespace cadise {

Photon::Photon() = default;

const Vector3R& Photon::position() const {
    return _position;
}

const Vector3R& Photon::inDirection() const {
    return _inDirection;
}

const Spectrum& Photon::throughputRadiance() const {
    return _throughputRadiance;
}

void Photon::setPosition(const Vector3R& position) {
    _position = position;
}

void Photon::setInDirection(const Vector3R& inDirection) {
    _inDirection = inDirection;
}

void Photon::setThroughputRadiance(const Spectrum& throughputRadiance) {
    _throughputRadiance = throughputRadiance;
}

} // namespace cadise