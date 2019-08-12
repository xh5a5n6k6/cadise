#pragma once

#include "core/intersector/primitive/primitive.h"

#include "math/matrix.h"

namespace cadise {

class Sphere : public Primitive {
public:
    Sphere(const std::shared_ptr<BSDF>& bsdf, const Vector3R& center, const real radius);

    AABB3R bound() const override;

    bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override;
    bool isOccluded(Ray& ray) const override;

    void evaluateSurfaceDetail(const PrimitiveInfo& primitiveInfo, SurfaceInfo& surfaceInfo) const override;

    void sampleSurface(const SurfaceInfo& inSurface, SurfaceInfo& outSurface) const override;
    real samplePdfA(const Vector3R& position) const override;
    real area() const override;

private:
    Matrix4 _worldToLocal;
    Vector3R _center;
    real _radius;

    std::shared_ptr<TextureMapper> _tmptextureMapper;
};

} // namespace cadise