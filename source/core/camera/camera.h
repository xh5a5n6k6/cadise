#pragma once

#include "math/fundamentalType.h"

namespace cadise {

class Film;
class Ray;

class Camera {
public:
    virtual Ray spawnPrimaryRay(const int32 px, const int32 py) const = 0;

    virtual Film film() const = 0;
};

} // namespace cadise