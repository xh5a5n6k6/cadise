#pragma once

#include "core/intersector/primitive/primitive.h"

#include "math/tVector3.h"

namespace cadise 
{

class Triangle : public Primitive 
{
public:
    Triangle(
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

    void setNa(const Vector3R& nA);
    void setNb(const Vector3R& nB);
    void setNc(const Vector3R& nC);

    void setUvwA(const Vector3R& uvwA);
    void setUvwB(const Vector3R& uvwB);
    void setUvwC(const Vector3R& uvwC);

private:
    /*
        Reference Note:
        https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates

        which is transcribed from "Real-Time Collision Detection"
    */
    void _positionToBarycentric(
        const Vector3R& position, 
        Vector3R* const out_barycentric) const;

    Vector3R _vA;
    Vector3R _vB;
    Vector3R _vC;

    Vector3R _nA;
    Vector3R _nB;
    Vector3R _nC;

    Vector3R _uvwA;
    Vector3R _uvwB;
    Vector3R _uvwC;

    Vector3R _eAB;
    Vector3R _eAC;
};

} // namespace cadise