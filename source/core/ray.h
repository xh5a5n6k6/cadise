#pragma once

#include "math/vector.h"

namespace cadise {

class Ray {
public:
    Ray();
    Ray(Vector3F origin, Vector3F direction, float minT, float maxT, int depth = 0);

    Vector3F at(float t);

    Vector3F origin();
    Vector3F direction();
    float minT();
    float maxT();
    int depth();
    float time();

    void setMinT(float minT);
    void setMaxT(float maxT);

private:
    Vector3F _origin, _direction;
    float _minT, _maxT;
    int _depth;
    float _time;
};

} // namespace cadise