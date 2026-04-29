#include "Core/Renderer/PhotonMapping/PMSetting.h"

#include "Foundation/Assertion.h"

namespace cadise
{

PMSetting::PMSetting(
    const std::size_t numPhotons,
    const real        searchRadius) :

    PMSetting(numPhotons, searchRadius, 0, 0.0001_r)
{}

PMSetting::PMSetting(
    const std::size_t numPhotons,
    const real        searchRadius,
    const std::size_t numIterations,
    const real        alpha) :

    _numPhotons(numPhotons),
    _searchRadius(searchRadius),
    _numIterations(numIterations),
    _alpha(alpha)
{
    CS_ASSERT_GT(numPhotons, 0);
    CS_ASSERT_GT(searchRadius, 0.0_r);
    CS_ASSERT_RANGE_INCLUSIVE(alpha, 0.0_r, 1.0_r);
}

std::size_t PMSetting::numPhotons() const
{
    return _numPhotons;
}

real PMSetting::searchRadius() const
{
    return _searchRadius;
}

std::size_t PMSetting::numIterations() const
{
    return _numIterations;
}

real PMSetting::alpha() const
{
    return _alpha;
}

} // namespace cadise