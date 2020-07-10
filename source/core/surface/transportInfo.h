#pragma once

#include "core/surface/bsdfComponents.h"
#include "core/surface/transportMode.h"

namespace cadise {

class TransportInfo {
public:
    TransportInfo();
    explicit TransportInfo(const TransportMode& mode);
    TransportInfo(
        const TransportMode& mode,
        const BsdfComponents components);

    void setComponents(const BsdfComponents components);

    const TransportMode& mode() const;
    BsdfComponents components() const;

private:
    TransportMode  _mode;
    BsdfComponents _components;
};

} // namespace cadise