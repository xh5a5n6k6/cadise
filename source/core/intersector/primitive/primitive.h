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
	
    virtual AABB3R bound() const override = 0;

    virtual bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override = 0;
    virtual bool isOccluded(const Ray& ray) const override = 0;

    virtual void evaluateSurfaceDetail(const PrimitiveInfo& primitiveInfo, SurfaceInfo& surfaceInfo) const = 0;

    virtual void sampleSurface(const SurfaceInfo& inSurface, SurfaceInfo& outSurface) const = 0;
    virtual real samplePdfA(const Vector3R& position) const = 0;
    virtual real area() const = 0;

    std::shared_ptr<Bsdf> bsdf() const;
    bool isEmissive() const;
    Spectrum emittance(const Vector3R& emitDirection, const SurfaceInfo& emitSurface) const;

    void setAreaLight(const std::shared_ptr<AreaLight>& areaLight);

protected:
    std::shared_ptr<Bsdf> _bsdf;
    std::shared_ptr<TextureMapper> _textureMapper;

    std::weak_ptr<AreaLight> _areaLight;
};

} // namespace cadise