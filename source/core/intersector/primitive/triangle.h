#pragma once

#include "core/intersector/primitive/primitive.h"

namespace cadise {

class Triangle : public Primitive {
public:
    Triangle(const std::shared_ptr<BSDF> bsdf, const Vector3R v1, const Vector3R v2, const Vector3R v3);

    AABB3R bound() const override;

    bool isIntersecting(Ray &ray, PrimitiveInfo &primitiveInfo) const override;
    bool isOccluded(Ray &ray) const override;

    void evaluateGeometryDetail(const PrimitiveInfo primitiveInfo, SurfaceGeometryInfo &geometryInfo) const override;
    void evaluteShadingDetail(SurfaceShadingInfo &shadingInfo) const override;

    void sampleSurface(const SurfaceGeometryInfo inSurface, SurfaceGeometryInfo &outSurface) const override;
    real samplePdfA(const Vector3R position) const override;
    real area() const override;

private:
    Vector3R _v1;
    Vector3R _v2;
    Vector3R _v3;

    Vector3R _e1;
    Vector3R _e2;
};

} // namespace cadise