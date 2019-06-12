#pragma once

#include "core/shape/shape.h"

#include "math/matrix.h"

namespace cadise {

class Sphere : public Shape {
public:
    Sphere(const Vector3R center, const real radius);

    AABB3R bound() const override;

    bool isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) const override;
    bool isOccluded(Ray &ray) const override;

private:
    Matrix4 _worldToLocal;
    Vector3R _center;
    real _radius;
};

} // namespace cadise