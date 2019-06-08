#pragma once

#include "math/type.h"

namespace cadise {

class Film;
class Ray;

class Camera {
public:
    virtual Ray createRay(int32 px, int32 py) = 0;

    virtual Film film() = 0;
};

} // namespace cadise