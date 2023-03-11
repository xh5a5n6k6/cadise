#pragma once

#include "Math/TVector2.h"

namespace cadise 
{

class Filter 
{
public:
    Filter(const real filterWidth, const real filterHeight);

    virtual ~Filter();

    virtual real evaluate(const real locationX, const real locationY) const = 0;

    const Vector2R& filterSize() const;
    Vector2R filterHalfSize() const;

protected:
    Vector2R _filterSize;
};

} // namespace cadise