#pragma once

#include "math/vector.h"

namespace cadise {

class Color {
};

class RGBColor : public Color {
public:
    RGBColor();
    RGBColor(real r, real g, real b);
    RGBColor(Vector3R rgb);

    Vector3R& rgb();
    real r();
    real g();
    real b();

private:
    Vector3R _rgb;
};

} // namespace cadise