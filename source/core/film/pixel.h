#pragma once

#include "math/vector.h"

namespace cadise {

class Pixel {
public:
    Pixel();

    void addValue(const Vector3R& value);
    void addWeight(const real weight);

    real r() const;
    real g() const;
    real b() const;
    real weight() const;

private:
    Vector3R _value;
    real _totalWeight;
};

} // namespace cadise