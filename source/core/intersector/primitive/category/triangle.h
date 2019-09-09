#pragma once

#include "core/intersector/primitive/primitive.h"

namespace cadise {

class Triangle : public Primitive {
public:
    Triangle(const std::shared_ptr<Bsdf>& bsdf, const Vector3R& vA, const Vector3R& vB, const Vector3R& vC);

    AABB3R bound() const override;

    bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override;
    bool isOccluded(Ray& ray) const override;

    void evaluateSurfaceDetail(const PrimitiveInfo& primitiveInfo, SurfaceInfo& surfaceInfo) const override;

    void sampleSurface(const SurfaceInfo& inSurface, SurfaceInfo& outSurface) const override;
    real samplePdfA(const Vector3R& position) const override;
    real area() const override;

    void setUvwA(const Vector3R& uvwA);
    void setUvwB(const Vector3R& uvwB);
    void setUvwC(const Vector3R& uvwC);

private:
    Vector3R _vA;
    Vector3R _vB;
    Vector3R _vC;

    Vector3R _uvwA;
    Vector3R _uvwB;
    Vector3R _uvwC;

    Vector3R _eA;
    Vector3R _eB;
};

} // namespace cadise