#include "core/integral-tool/connectEvent.h"

namespace cadise 
{

ConnectEvent::ConnectEvent() :
    ConnectEvent(Vector2D(0.0_r), Spectrum(0.0_r))
{}

ConnectEvent::ConnectEvent(
    const Vector2D& filmPosition,
    const Spectrum& splatRadiance) :

    _filmPosition(filmPosition),
    _splatRadiance(splatRadiance) 
{}

const Vector2D& ConnectEvent::filmPosition() const 
{
    return _filmPosition;
}

const Spectrum& ConnectEvent::splatRadiance() const 
{
    return _splatRadiance;
}

void ConnectEvent::setFilmPosition(const Vector2D& filmPosition)
{
    _filmPosition = filmPosition;
}

void ConnectEvent::setSplatRadiance(const Spectrum& splatRadiance) 
{
    _splatRadiance = splatRadiance;
}

} // namespace cadise