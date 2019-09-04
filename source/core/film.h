#pragma once

#include "file-io/path.h"

#include "math/vector.h"

namespace cadise {

class Film {
public:
    Film(const Path& filename, const Vector2I& resolution);

    void addSample(const int32 px, const int32 py, const Vector3R& value);
    void save() const;

    Vector2I resolution() const;

private:
    Path _filename;
    Vector2I _resolution;
    real* _pixelValue;
};

} // namespace cadise