#pragma once

#include "Core/Surface/BSDFComponents.h"
#include "Core/Surface/ETransportMode.h"

namespace cadise
{

class TransportInfo
{
public:
    TransportInfo();
    explicit TransportInfo(const ETransportMode mode);
    TransportInfo(
        const ETransportMode mode,
        const BSDFComponents components);

    void setComponents(const BSDFComponents components);

    ETransportMode mode() const;
    BSDFComponents components() const;

private:
    ETransportMode _mode;
    BSDFComponents _components;
};

} // namespace cadise