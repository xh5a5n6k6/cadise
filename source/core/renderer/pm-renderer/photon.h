#pragma once

#include "core/spectrum/spectrum.h"

namespace cadise {

class Photon {
public:
    Photon();

    const Vector3R& position() const;
    const Vector3R& inDirection() const;
    const Spectrum& throughputRadiance() const;

    void setPosition(const Vector3R& position);
    void setInDirection(const Vector3R& inDirection);
    void setThroughputRadiance(const Spectrum& throughputRadiance);

private:
    Vector3R _position;
    Vector3R _inDirection;
    Spectrum _throughputRadiance;
};

} // namespace cadise