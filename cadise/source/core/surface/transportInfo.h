#pragma once

#include "core/surface/bsdfComponents.h"
#include "core/surface/eTransportMode.h"

namespace cadise {

class TransportInfo {
public:
    TransportInfo();
    explicit TransportInfo(const ETransportMode mode);
    TransportInfo(
        const ETransportMode mode,
        const BsdfComponents components);

    void setComponents(const BsdfComponents components);

    ETransportMode mode() const;
    BsdfComponents components() const;

private:
    ETransportMode _mode;
    BsdfComponents _components;
};

} // namespace cadise