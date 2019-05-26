#pragma once

#include "core/shape/shape.h"

#include "math/matrix.h"

namespace cadise {

class Sphere : public Shape {
public:
    Sphere(Vector3F center, float radius);

    AABB3F bound() override;

    bool isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) override;
    bool isOccluded(Ray &ray) override;

private:
    Matrix4 _worldToLocal;
    Vector3F _center;
    float _radius;
};

} // namespace cadise