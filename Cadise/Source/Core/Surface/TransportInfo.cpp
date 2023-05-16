#include "Core/Surface/TransportInfo.h"

namespace cadise
{

TransportInfo::TransportInfo() :
    TransportInfo(ETransportMode::Radiance)
{}

TransportInfo::TransportInfo(const ETransportMode mode) :
    TransportInfo(mode, BSDF_ALL_COMPONENTS)
{}

TransportInfo::TransportInfo(
    const ETransportMode mode,
    const BSDFComponents components) :

    _mode(mode),
    _components(components)
{}

void TransportInfo::setComponents(const BSDFComponents components)
{
    _components = components;
}

ETransportMode TransportInfo::mode() const
{
    return _mode;
}

BSDFComponents TransportInfo::components() const
{
    return _components;
}

} // namespace cadise