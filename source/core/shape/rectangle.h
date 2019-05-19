#pragma once

#include "core/shape/shape.h"

#include "math/algebra.h"

namespace cadise {

class Rectangle : public Shape {
public:
    Rectangle(Vector3 v1, Vector3 v2, Vector3 v3);

    virtual bool isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) override;
    virtual bool isOccluded(Ray &ray) override;
    virtual Vector3 sampleSurfacePoint() override;

private:
    Vector3 _vertex[3];
};

} // namespace cadise