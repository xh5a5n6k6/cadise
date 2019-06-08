#pragma once

#include "core/shape/shape.h"

#include "math/matrix.h"

namespace cadise {

class Sphere : public Shape {
public:
    Sphere(Vector3R center, real radius);

    AABB3R bound() override;

    bool isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) override;
    bool isOccluded(Ray &ray) override;

private:
    Matrix4 _worldToLocal;
    Vector3R _center;
    real _radius;
};

} // namespace cadise