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
	
    AABB3R bound() const override = 0;

    bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override = 0;
    bool isOccluded(const Ray& ray) const override = 0;

    virtual void evaluateSurfaceDetail(const PrimitiveInfo& primitiveInfo, SurfaceInfo& surfaceInfo) const = 0;

    virtual void sampleSurface(const SurfaceInfo& inSurface, SurfaceInfo& outSurface) const = 0;
    virtual real samplePdfA(const Vector3R& position) const = 0;
    virtual real area() const = 0;

    const Bsdf* bsdf() const;
    const AreaLight* areaLight() const;
    void setAreaLight(const AreaLight* const areaLight);

protected:
    std::shared_ptr<Bsdf> _bsdf;
    std::shared_ptr<TextureMapper> _textureMapper;
    
    const AreaLight* _areaLight;
};

} // namespace cadise