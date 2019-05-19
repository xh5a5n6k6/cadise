#include "core/shape/shape.h"

#include "math/algebra.h"

#include <iostream>

namespace cadise {

Vector3 Shape::sampleSurfacePoint() {
    fprintf(stderr, "This shape of area light doesn't support\n");

    return Vector3(0.0f, 0.0f, 0.0f);
}

} // namespace cadise