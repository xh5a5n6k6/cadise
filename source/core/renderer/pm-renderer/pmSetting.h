#pragma once

#include "math/type/fundamentalType.h"

namespace cadise {

class PmSetting {
public:
    PmSetting(
        const std::size_t numPhotons,
        const std::size_t numIterations,
        const real        searchRadius);

    std::size_t numPhotons() const;
    std::size_t numIterations() const;
    real searchRadius() const;

private:
    // number of photons stored in photon map per iteration
    std::size_t _numPhotons;

    std::size_t _numIterations;
    real        _searchRadius;
};

} // namespace cadise