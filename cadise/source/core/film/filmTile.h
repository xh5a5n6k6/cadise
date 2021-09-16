#pragma once

#include "core/film/rgbRadianceSensor.h"
#include "core/spectrum/spectrum.h"
#include "math/tAabb2.h"

#include <vector>

namespace cadise { class Filter; }

namespace cadise 
{

class FilmTile 
{
public:
    FilmTile(const AABB2I& tileBound, const Filter* const filter);

    void addSample(const Vector2R& filmPosition, const Spectrum& value);
    void addSample(const Vector2R& filmPosition, const Vector3R& value);

    const AABB2I& tileBound() const;
    const RgbRadianceSensor& getSensor(const int32 x, const int32 y) const;

    void setSensorSize(const std::size_t sensorSize);

private:
    std::size_t _sensorIndexOffset(const int32 x, const int32 y) const;

    const Filter* _filter;

    Vector2I                       _resolution;
    AABB2I                         _tileBound;
    std::vector<RgbRadianceSensor> _sensors;
};

} // namespace cadise