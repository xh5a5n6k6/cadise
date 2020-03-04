#include "core/surface/transportInfo.h"

namespace cadise {

TransportInfo::TransportInfo() :
    TransportInfo(TransportMode::RADIANCE) {
}

TransportInfo::TransportInfo(const TransportMode& mode) :
    _mode(mode) {
}

const TransportMode& TransportInfo::mode() const {
    return _mode;
}

} // namespace cadise