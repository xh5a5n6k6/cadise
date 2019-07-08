#pragma once

#include "core/intersector/intersector.h"

#include <memory>

namespace cadise {

class AreaLight;
class BSDF;
class RGBColor;
class SurfaceGeometryInfo;
class SurfaceShadingInfo;

class Primitive : public Intersector {
public:
    Primitive();
    Primitive(const std::shared_ptr<BSDF>& bsdf);
	
    virtual AABB3R bound() const override = 0;

    virtual bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override = 0;
    virtual bool isOccluded(Ray& ray) const override = 0;

    virtual void evaluateGeometryDetail(const PrimitiveInfo& primitiveInfo, SurfaceGeometryInfo& geometryInfo) const = 0;
    virtual void evaluteShadingDetail(SurfaceShadingInfo& shadingInfo) const = 0;

    virtual void sampleSurface(const SurfaceGeometryInfo& inSurface, SurfaceGeometryInfo& outSurface) const = 0;
    virtual real samplePdfA(const Vector3R& position) const = 0;
    virtual real area() const = 0;

    std::shared_ptr<BSDF> bsdf() const;
    RGBColor emittance(const Vector3R& outDirection) const;

    void setAreaLight(const std::shared_ptr<AreaLight>& areaLight);

protected:
    std::shared_ptr<BSDF> _bsdf;
    std::weak_ptr<AreaLight> _areaLight;
};

} // namespace cadise