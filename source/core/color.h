#pragma once

#include "math/vector.h"

namespace cadise {

class Color {
};

class RGBColor : public Color {
public:
    RGBColor();
    RGBColor(float r, float g, float b);
    RGBColor(Vector3F rgb);

    Vector3F& rgb();
    float r();
    float g();
    float b();

private:
    Vector3F _rgb;
};

} // namespace cadise