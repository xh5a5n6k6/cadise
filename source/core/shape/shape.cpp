#include "core/shape/shape.h"

#include "core/surfaceInfo.h"

#include <iostream>

namespace cadise {

void Shape::sampleSurface(const SurfaceInfo inSurface, SurfaceInfo &outSurface) const {
    std::cerr << "This shape of area light doesn't support\n";
}

real Shape::samplePdfA() const {
    std::cerr << "This shape of area light doesn't support\n";

    return 0.0_r;
}

real Shape::area() const {
    std::cerr << "This shape of area light doesn't support\n";

    return 0.0_r;
}

} // namespace cadise