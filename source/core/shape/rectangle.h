#pragma once

#include "core/shape/shape.h"

namespace cadise {

class Rectangle : public Shape {
public:
    Rectangle(Vector3R v1, Vector3R v2, Vector3R v3);

    AABB3R bound() override;

    bool isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) override;
    bool isOccluded(Ray &ray) override;

    void sampleSurface(SurfaceInfo inSurface, SurfaceInfo &outSurface) override;
    real samplePdfA() override;

    real area() override;

private:
    Vector3R _v1;
    Vector3R _v2;
    Vector3R _v3;

    Vector3R _e1;
    Vector3R _e2;
};

} // namespace cadise