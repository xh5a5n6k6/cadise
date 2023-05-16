#pragma once

#include "Core/Spectrum/Spectrum.h"
#include "Math/TVector2.h"

namespace cadise
{

class ConnectEvent
{
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