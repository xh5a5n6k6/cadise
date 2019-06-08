#pragma once

#include "math/vector.h"

namespace cadise {

class Ray {
public:
    Ray();
    Ray(Vector3R origin, Vector3R direction, real minT, real maxT, int32 depth = 0);

    Vector3R at(real t);

    Vector3R origin();
    Vector3R direction();
    real minT();
    real maxT();
    int32 depth();

    void setMinT(real minT);
    void setMaxT(real maxT);

private:
    Vector3R _origin;
    Vector3R _direction;

    real _minT;
    real _maxT;
    int32 _depth;
};

} // namespace cadise