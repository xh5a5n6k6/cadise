#pragma once

#include "core/intersector/primitive/primitive.h"

#include "math/tVector3.h"

namespace cadise {

class Rectangle : public Primitive {
public:
    Rectangle(
        const std::shared_ptr<Bsdf>& bsdf, 
        const Vector3R&              vA, 
        const Vector3R&              vB, 
        const Vector3R&              vC);

    void evaluateBound(AABB3R* const out_bound) const override;

    bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override;
    bool isOccluded(const Ray& ray) const override;

    void evaluateSurfaceDetail(
        const PrimitiveInfo& primitiveInfo, 
        SurfaceDetail* const out_surface) const override;

    void evaluatePositionSample(PositionSample* const out_sample) const override;
    real evaluatePositionPdfA(const Vector3R& position) const override;
    real area() const override;

    void setUvwA(const Vector3R& uvwA);
    void setUvwB(const Vector3R& uvwB);
    void setUvwC(const Vector3R& uvwC);
    void setUvwD(const Vector3R& uvwD);

private:
    Vector3R _vA;
    Vector3R _vB;
    Vector3R _vC;
    Vector3R _vD;

    Vector3R _uvwA;
    Vector3R _uvwB;
    Vector3R _uvwC;
    Vector3R _uvwD;

    Vector3R _eA;
    Vector3R _eB;
};

} // namespace cadise