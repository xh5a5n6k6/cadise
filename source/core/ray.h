#pragma once

#include "math/vector.h"

namespace cadise {

class Ray {
public:
    Ray();
    Ray(const Vector3R origin, const Vector3R direction, const real minT, const real maxT, const int32 depth = 0);

    Vector3R at(const real t) const;

    Vector3R origin() const;
    Vector3R direction() const;
    real minT() const;
    real maxT() const;
    int32 depth() const;

    void setMinT(const real minT);
    void setMaxT(const real maxT);

private:
    Vector3R _origin;
    Vector3R _direction;

    real _minT;
    real _maxT;
    int32 _depth;
};

} // namespace cadise