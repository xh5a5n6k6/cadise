#pragma once

#include "Math/Type/MathType.h"

namespace cadise 
{

/*
    It stores radiance as rgb value rather than origin spectrum.
*/
class RGBRadianceSensor
{
public:
    RGBRadianceSensor();

    void addValue(const Vector3R& value);
    void addValue(const real r, const real g, const real b);

    void addWeight(const real weight);

    real r() const;
    real g() const;
    real b() const;
    real weight() const;

private:
    real _r;
    real _g;
    real _b;
    real _weight;
};

} // namespace cadise