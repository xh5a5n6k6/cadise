#pragma once

#include "core/shape/shape.h"

#include "math/vector.h"

namespace cadise {

class Triangle : public Shape {
public:
    Triangle(Vector3 v1, Vector3 v2, Vector3 v3);

    bool isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) override;
    bool isOccluded(Ray &ray) override;
    Vector3 sampleSurfacePoint() override;

private:
    Vector3 _vertex[3];
};

} // namespace cadise