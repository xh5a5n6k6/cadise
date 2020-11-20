#include "core/renderer/vanilla-pm/vpmSetting.h"

#include "fundamental/assertion.h"

namespace cadise {

VpmSetting::VpmSetting(
    const std::size_t numPhotons,
    const real        searchRadius) :

    _numPhotons(numPhotons),
    _searchRadius(searchRadius) {

    CADISE_ASSERT_GT(searchRadius, 0.0_r);
}

std::size_t VpmSetting::numPhotons() const {
    return _numPhotons;
}

real VpmSetting::searchRadius() const {
    return _searchRadius;
}

} // namespace cadise