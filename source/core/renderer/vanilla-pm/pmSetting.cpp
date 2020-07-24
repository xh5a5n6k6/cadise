#include "core/renderer/vanilla-pm/pmSetting.h"

#include "fundamental/assertion.h"

namespace cadise {

PmSetting::PmSetting(
    const std::size_t numPhotons,
    const std::size_t numIterations,
    const real        searchRadius) :

    _numPhotons(numPhotons),
    _numIterations(numIterations),
    _searchRadius(searchRadius) {

    CADISE_ASSERT_GT(searchRadius, 0.0_r);
}

std::size_t PmSetting::numPhotons() const {
    return _numPhotons;
}

std::size_t PmSetting::numIterations() const {
    return _numIterations;
}

real PmSetting::searchRadius() const {
    return _searchRadius;
}

} // namespace cadise