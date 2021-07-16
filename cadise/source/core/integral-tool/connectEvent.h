#pragma once

#include "core/spectrum/spectrum.h"
#include "math/tVector2.h"

namespace cadise {

class ConnectEvent {
public:
    ConnectEvent();
    ConnectEvent(
        const Vector2D& filmPosition, 
        const Spectrum& splatRadiance);

    const Vector2D& filmPosition() const;
    const Spectrum& splatRadiance() const;

    void setFilmPosition(const Vector2D& filmPosition);
    void setSplatRadiance(const Spectrum& splatRadiance);

private:
    Vector2D _filmPosition;
    Spectrum _splatRadiance;
};

} // namespace cadise