#pragma once

#include "core/shape/shape.h"

#include "math/vector.h"

namespace cadise {

class Triangle : public Shape {
public:
    Triangle(Vector3F v1, Vector3F v2, Vector3F v3);

    bool isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) override;
    bool isOccluded(Ray &ray) override;
    void sampleSurface(SurfaceInfo inSurface, SurfaceInfo &outSurface) override;

private:
    Vector3F _vertex[3];
};

} // namespace cadise