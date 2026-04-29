#pragma once

#include "Core/Intersector/Intersector.h"

#include "Core/Spectrum/Spectrum.h"

#include <memory>

// forward declaration
namespace cadise
{
    class AreaLight;
    class BSDF;
    class PositionSample;
    class SurfaceDetail;
    class TextureMapper;
}

namespace cadise
{

class Primitive : public Intersector
{
public:
    Primitive();
    explicit Primitive(const std::shared_ptr<BSDF>& bsdf);

    void evaluateBound(AABB3R* const out_bound) const override = 0;

    bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override = 0;
    bool isOccluded(const Ray& ray) const override = 0;

    virtual void evaluateSurfaceDetail(
        const PrimitiveInfo& primitiveInfo,
        SurfaceDetail* const out_surface) const = 0;

    virtual void evaluatePositionSample(PositionSample* const out_sample) const;
    virtual real evaluatePositionPdfA(const Vector3R& position) const;
    virtual real area() const;

    virtual void uvwToPosition(
        const Vector3R& uvw,
        Vector3R* const out_position) const;

    const BSDF* bsdf() const;
    const AreaLight* areaLight() const;
    void setAreaLight(const AreaLight* const areaLight);

protected:
    std::shared_ptr<BSDF>          _bsdf;
    std::shared_ptr<TextureMapper> _textureMapper;

    const AreaLight* _areaLight;
};

} // namespace cadise