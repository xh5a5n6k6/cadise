#pragma once

#include "core/spectrum/spectrum.h"

namespace cadise {

class ConnectEvent {
public:
    ConnectEvent();
    ConnectEvent(
        const Vector2R& filmNdcPosition, 
        const Spectrum& splatRadiance);

    const Vector2R& filmNdcPosition() const;
    const Spectrum& splatRadiance() const;

    void setFilmNdcPosition(const Vector2R& filmNdcPosition);
    void setSplatRadiance(const Spectrum& splatRadiance);

private:
    Vector2R _filmNdcPosition;
    Spectrum _splatRadiance;
};

} // namespace cadise