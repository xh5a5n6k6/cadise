#pragma once

#include "math/vector.h"

namespace cadise {

class Color {
};

class RGBColor : public Color {
public:
    RGBColor();
    RGBColor(const real r, const real g, const real b);
    RGBColor(const Vector3R& rgb);

    Vector3R& rgb();
    real r() const;
    real g() const;
    real b() const;

private:
    Vector3R _rgb;
};

} // namespace cadise