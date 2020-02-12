#pragma once

#include "core/intersector/intersector.h"

#include "core/spectrum/spectrum.h"

#include <memory>

namespace cadise {

class AreaLight;
class Bsdf;
class SurfaceInfo;
class TextureMapper;

class Primitive : public Intersector {
public:
    Primitive();
    Primitive(const std::shared_ptr<Bsdf>& bsdf);
	
    void evaluateBound(AABB3R* const out_bound) const override = 0;

    bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override = 0;
    bool isOccluded(const Ray& ray) const override = 0;

    virtual void evaluateSurfaceDetail(
        const PrimitiveInfo& primitiveInfo, 
        SurfaceInfo* const   out_surface) const = 0;

    virtual void sampleSurface(
        const SurfaceInfo& inSurface, 
        SurfaceInfo* const out_surface) const;

    virtual real samplePdfA(const Vector3R& position) const;
    virtual real area() const;

    virtual void uvwToPosition(
        const Vector3R& uvw, 
        Vector3R* const out_position) const;

    const Bsdf* bsdf() const;
    const AreaLight* areaLight() const;
    void setAreaLight(const AreaLight* const areaLight);

protected:
    std::shared_ptr<Bsdf> _bsdf;
    std::shared_ptr<TextureMapper> _textureMapper;
    
    const AreaLight* _areaLight;
};

} // namespace cadise