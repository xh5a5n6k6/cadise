#pragma once

#include "math/vector.h"

namespace cadise {

class Ray {
public:
    Ray();
    Ray(Vector3 origin, Vector3 direction, float minT, float maxT, int depth = 0);

    Vector3 at(float t);

    Vector3 origin();
    Vector3 direction();
    float minT();
    float maxT();
    int depth();
    float time();

    void setMinT(float minT);
    void setMaxT(float maxT);

private:
    Vector3 _origin, _direction;
    float _minT, _maxT;
    int _depth;
    float _time;
};

} // namespace cadise