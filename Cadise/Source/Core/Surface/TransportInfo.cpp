#include "core/surface/transportInfo.h"

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
    const BsdfComponents components) :

    _mode(mode),
    _components(components)
{}

void TransportInfo::setComponents(const BsdfComponents components) 
{
    _components = components;
}

ETransportMode TransportInfo::mode() const
{
    return _mode;
}

BsdfComponents TransportInfo::components() const 
{
    return _components;
}

} // namespace cadise