#include "core/surface/transportInfo.h"

namespace cadise {

TransportInfo::TransportInfo() :
    TransportInfo(TransportMode::RADIANCE) {
}

TransportInfo::TransportInfo(const TransportMode& mode) :
    TransportInfo(mode, BSDF_ALL_COMPONENTS) {
}

TransportInfo::TransportInfo(
    const TransportMode& mode,
    const BsdfComponents components) :

    _mode(mode),
    _components(components) {
}

void TransportInfo::setComponents(const BsdfComponents components) {
    _components = components;
}

const TransportMode& TransportInfo::mode() const {
    return _mode;
}

BsdfComponents TransportInfo::components() const {
    return _components;
}

} // namespace cadise