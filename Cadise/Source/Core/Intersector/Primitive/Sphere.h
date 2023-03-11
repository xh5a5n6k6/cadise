#pragma once

#include "Core/Intersector/Primitive/Primitive.h"

#include "Math/TVector3.h"

// forward declaration
namespace cadise
{
    class Transform;
}

namespace cadise 
{

class Sphere : public Primitive 
{
public:
    Sphere(
        const std::shared_ptr<BSDF>& bsdf, 
        const Vector3R&              center, 
        const real                   radius);

    void evaluateBound(AABB3R* const out_bound) const override;

    bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override;
    bool isOccluded(const Ray& ray) const override;

    void evaluateSurfaceDetail(
        const PrimitiveInfo& primitiveInfo, 
        SurfaceDetail* const out_surface) const override;

    void evaluatePositionSample(PositionSample* const out_sample) const override;
    real evaluatePositionPdfA(const Vector3R& position) const override;
    real area() const override;

private:
    bool _isSolutionValid(
        const real  t0,
        const real  t1,
        const real  minT,
        const real  maxT,
        real* const out_finalT) const;

    std::shared_ptr<Transform> _worldToLocal;

    Vector3R _center;
    real     _radius;

    // HACK, TODO: remove this
    std::shared_ptr<TextureMapper> _tmptextureMapper;
};

} // namespace cadise