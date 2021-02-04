#include "core/renderer/photon-mapping/pmSetting.h"

#include "fundamental/assertion.h"

namespace cadise {

PmSetting::PmSetting(
    const std::size_t numPhotons,
    const real        searchRadius) :

    PmSetting(numPhotons, searchRadius, 0, 0.0001_r) {
}

PmSetting::PmSetting(
    const std::size_t numPhotons,
    const real        searchRadius,
    const std::size_t numIterations,
    const real        alpha) :

    _numPhotons(numPhotons),
    _searchRadius(searchRadius),
    _numIterations(numIterations),
    _alpha(alpha) {

    CADISE_ASSERT_GT(numPhotons, 0);
    CADISE_ASSERT_GT(searchRadius, 0.0_r);
    CADISE_ASSERT_RANGE_INCLUSIVE(alpha, 0.0_r, 1.0_r);
}

std::size_t PmSetting::numPhotons() const {
    return _numPhotons;
}

real PmSetting::searchRadius() const {
    return _searchRadius;
}

std::size_t PmSetting::numIterations() const {
    return _numIterations;
}

real PmSetting::alpha() const {
    return _alpha;
}

} // namespace cadise