#pragma once

#include "math/vector.h"

namespace cadise {

class Color {
};

class RGBColor : public Color {
public:
    RGBColor();
    RGBColor(float r, float g, float b);
    RGBColor(Vector3 rgb);

    Vector3& rgb();
    float r();
    float g();
    float b();

private:
    Vector3 _rgb;
};

} // namespace cadise