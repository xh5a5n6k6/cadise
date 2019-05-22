#pragma once

#include "file-io/path.h"

namespace cadise {

class Vector3;

class Film {
public:
    Film();
    Film(Path filename, int rx, int ry);

    void addSample(int px, int py, Vector3 value);
    void save();

    int resolutionX();
    int resolutionY();

private:
    Path _filename;
    int _resolutionX, _resolutionY;
    float* _pixelValue;
};

} // namespace cadise