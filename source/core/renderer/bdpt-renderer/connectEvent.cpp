#include "core/renderer/bdpt-renderer/connectEvent.h"

namespace cadise {

ConnectEvent::ConnectEvent() :
    ConnectEvent(Vector2R(0.0_r), Spectrum(0.0_r)){
}

ConnectEvent::ConnectEvent(
    const Vector2R& filmNdcPosition,
    const Spectrum& splatRadiance) :

    _filmNdcPosition(filmNdcPosition),
    _splatRadiance(splatRadiance) {
}

const Vector2R& ConnectEvent::filmNdcPosition() const {
    return _filmNdcPosition;
}

const Spectrum& ConnectEvent::splatRadiance() const {
    return _splatRadiance;
}

void ConnectEvent::setFilmNdcPosition(const Vector2R& filmNdcPosition) {
    _filmNdcPosition = filmNdcPosition;
}

void ConnectEvent::setSplatRadiance(const Spectrum& splatRadiance) {
    _splatRadiance = splatRadiance;
}

} // namespace cadise