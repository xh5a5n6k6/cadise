#pragma once

#include "math/vector.h"

namespace cadise {

class Bsdf;

class PathVertex {
public:

private:
    Vector3R _position;
    
    real _pdfForward;
    real _pdfReverse;

    const Bsdf* _bsdf;
};

} // namespace cadise