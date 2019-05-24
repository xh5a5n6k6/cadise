#pragma once

#include "file-io/path.h"

#include "math/vector.h"

namespace cadise {

class Film {
public:
    Film();
    Film(Path filename, int rx, int ry);

    void addSample(int px, int py, Vector3F value);
    void save();

    Vector2I resolution();

private:
    Path _filename;
    Vector2I _resolution;
    float* _pixelValue;
};

} // namespace cadise