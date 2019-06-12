#pragma once

#include "math/type.h"

namespace cadise {

class Film;
class Ray;

class Camera {
public:
    virtual Ray createRay(const int32 px, const int32 py) const = 0;

    virtual Film film() const = 0;
};

} // namespace cadise