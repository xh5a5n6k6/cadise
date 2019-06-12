#pragma once

#include "core/shape/shape.h"

namespace cadise {

class Rectangle : public Shape {
public:
    Rectangle(const Vector3R v1, const Vector3R v2, const Vector3R v3);

    AABB3R bound() const override;

    bool isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) const override;
    bool isOccluded(Ray &ray) const override;

    void sampleSurface(const SurfaceInfo inSurface, SurfaceInfo &outSurface) const override;
    real samplePdfA() const override;

    real area() const override;

private:
    Vector3R _v1;
    Vector3R _v2;
    Vector3R _v3;

    Vector3R _e1;
    Vector3R _e2;
};

} // namespace cadise