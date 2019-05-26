#include "core/shape/shape.h"

#include "core/surfaceInfo.h"

#include "math/vector.h"

#include <iostream>

namespace cadise {

void Shape::sampleSurface(SurfaceInfo inSurface, SurfaceInfo &outSurface) {
    std::cerr << "This shape of area light doesn't support\n";
}

float Shape::samplePdf() {
    std::cerr << "This shape of area light doesn't support\n";

    return 0.0f;
}

float Shape::area() {
    std::cerr << "This shape of area light doesn't support\n";

    return 0.0f;
}

} // namespace cadise