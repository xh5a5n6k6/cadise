#pragma once

#include "core/intersector/primitive/primitive.h"

namespace cadise {

class Transform;

class Sphere : public Primitive {
public:
    Sphere(const std::shared_ptr<Bsdf>& bsdf, const Vector3R& center, const real radius);

    void evaluateBound(AABB3R* const out_bound) const override;

    bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override;
    bool isOccluded(const Ray& ray) const override;

    void evaluateSurfaceDetail(const PrimitiveInfo& primitiveInfo, SurfaceInfo& surfaceInfo) const override;

    void sampleSurface(const SurfaceInfo& inSurface, SurfaceInfo* const out_surface) const override;
    real samplePdfA(const Vector3R& position) const override;
    real area() const override;

private:
    std::shared_ptr<Transform> _worldToLocal;
    Vector3R _center;
    real _radius;

    std::shared_ptr<TextureMapper> _tmptextureMapper;
};

} // namespace cadise