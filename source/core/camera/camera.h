#pragma once

#include "math/mathType.h"

namespace cadise {

class Film;
class Ray;

class Camera {
public:
    virtual Ray spawnPrimaryRay(const Vector2I& pixelIndex, const Vector2R& sample) const = 0;

    virtual Film film() const = 0;
};

} // namespace cadise