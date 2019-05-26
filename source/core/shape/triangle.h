#pragma once

#include "core/shape/shape.h"

namespace cadise {

class Triangle : public Shape {
public:
    Triangle(Vector3F v1, Vector3F v2, Vector3F v3);

    AABB3F bound() override;

    bool isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) override;
    bool isOccluded(Ray &ray) override;

    void sampleSurface(SurfaceInfo inSurface, SurfaceInfo &outSurface) override;

private:
    Vector3F _v1;
    Vector3F _v2;
    Vector3F _v3;

    Vector3F _e1;
    Vector3F _e2;
};

} // namespace cadise