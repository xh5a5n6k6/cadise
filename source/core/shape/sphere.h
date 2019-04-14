#pragma once

#include "core/shape/shape.h"

#include "math/algebra.h"

namespace cadise {

class Sphere : public Shape {
public:
    Sphere(Vector3 center, float radius);

    bool isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) override;
    bool isOccluded(Ray &ray) override;

    Matrix4 _worldToLocal;
    Vector3 _center;
    float _radius;
};

} // namespace cadise