#pragma once

#include "core/spectrum/spectrum.h"
#include "math/tVector3.h"

namespace cadise 
{

class Photon 
{
public:
    Photon();

    const Vector3R& position() const;
    const Vector3R& fromDirection() const;
    const Spectrum& throughputRadiance() const;

    void setPosition(const Vector3R& position);
    void setFromDirection(const Vector3R& fromDirection);
    void setThroughputRadiance(const Spectrum& throughputRadiance);

private:
    Vector3R _position;
    Vector3R _fromDirection;
    Spectrum _throughputRadiance;
};

} // namespace cadise