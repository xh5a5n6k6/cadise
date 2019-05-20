#include "core/shape/shape.h"

#include "math/vector.h"

#include <iostream>

namespace cadise {

Vector3 Shape::sampleSurfacePoint() {
    std::cerr << "This shape of area light doesn't support\n";

    return Vector3(0.0f, 0.0f, 0.0f);
}

} // namespace cadise