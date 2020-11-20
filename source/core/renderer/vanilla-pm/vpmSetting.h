#pragma once

#include "math/type/fundamentalType.h"

namespace cadise {

class VpmSetting {
public:
    VpmSetting(
        const std::size_t numPhotons,
        const real        searchRadius);

    std::size_t numPhotons() const;
    real searchRadius() const;

private:
    std::size_t _numPhotons;
    real        _searchRadius;
};

} // namespace cadise