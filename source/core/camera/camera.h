#pragma once

#include "math/mathType.h"

namespace cadise {

class Film;
class Ray;

class Camera {
public:
    virtual Ray spawnPrimaryRay(const Vector2R& pixelPosition) const = 0;

    virtual Film film() const = 0;
};

} // namespace cadise