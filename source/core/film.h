#pragma once

#include "file-io/path.h"

#include "math/vector.h"

namespace cadise {

class Film {
public:
    Film(Path filename, int32 rx, int32 ry);

    void addSample(int32 px, int32 py, Vector3R value);
    void save();

    Vector2I resolution();

private:
    Path _filename;
    Vector2I _resolution;
    real* _pixelValue;
};

} // namespace cadise