#pragma once

#include "Math/Type/PrimitiveType.h"

namespace cadise
{

class PMSetting
{
public:
    PMSetting(
        const std::size_t numPhotons,
        const real        searchRadius);

    PMSetting(
        const std::size_t numPhotons,
        const real        searchRadius,
        const std::size_t numIterations,
        const real        alpha);

    std::size_t numPhotons() const;
    real searchRadius() const;
    std::size_t numIterations() const;
    real alpha() const;

private:
    std::size_t _numPhotons;
    real        _searchRadius;
    std::size_t _numIterations;
    real        _alpha;
};

} // namespace cadise