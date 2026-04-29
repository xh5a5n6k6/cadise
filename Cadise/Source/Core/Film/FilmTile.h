#pragma once

#include "Core/Film/RGBRadianceSensor.h"
#include "Core/Spectrum/Spectrum.h"
#include "Math/TAabb2.h"

#include <vector>

// forward declaration
namespace cadise
{
    class Filter;
}

namespace cadise
{

class FilmTile
{
public:
    FilmTile(const AABB2I& tileBound, const Filter* const filter);

    void addSample(const Vector2R& filmPosition, const Spectrum& value);
    void addSample(const Vector2R& filmPosition, const Vector3R& value);

    const AABB2I& tileBound() const;
    const RGBRadianceSensor& getSensor(const int32 x, const int32 y) const;

    void setSensorSize(const std::size_t sensorSize);

private:
    std::size_t _sensorIndexOffset(const int32 x, const int32 y) const;

    const Filter* _filter;

    Vector2I                       _resolution;
    AABB2I                         _tileBound;
    std::vector<RGBRadianceSensor> _sensors;
};

} // namespace cadise