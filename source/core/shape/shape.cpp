#include "core/shape/shape.h"

#include "core/surfaceInfo.h"

#include <iostream>

namespace cadise {

void Shape::sampleSurface(SurfaceInfo inSurface, SurfaceInfo &outSurface) {
    std::cerr << "This shape of area light doesn't support\n";
}

real Shape::samplePdfA() {
    std::cerr << "This shape of area light doesn't support\n";

    return 0.0_r;
}

real Shape::area() {
    std::cerr << "This shape of area light doesn't support\n";

    return 0.0_r;
}

} // namespace cadise