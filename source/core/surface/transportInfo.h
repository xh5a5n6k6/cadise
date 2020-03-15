#pragma once

#include "core/surface/transportMode.h"

namespace cadise {

class TransportInfo {
public:
    TransportInfo();
    explicit TransportInfo(const TransportMode& mode);

    const TransportMode& mode() const;

private:
    TransportMode _mode;

    // TODO: add lobe mask (check)
};

} // namespace cadise