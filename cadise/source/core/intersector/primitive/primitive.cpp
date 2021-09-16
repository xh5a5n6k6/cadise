#include "core/intersector/primitive/primitive.h"

#include "fundamental/assertion.h"

namespace cadise 
{

Primitive::Primitive() = default;

Primitive::Primitive(const std::shared_ptr<Bsdf>& bsdf) :
    _bsdf(bsdf),
    _textureMapper(nullptr),
    _areaLight(nullptr) 
{
    CADISE_ASSERT(bsdf);
}

void Primitive::evaluatePositionSample(PositionSample* const out_sample) const 
{
    CADISE_ASSERT(out_sample);

    return;
}

real Primitive::evaluatePositionPdfA(const Vector3R& position) const 
{
    return 0.0_r;
}

real Primitive::area() const 
{
    return 0.0_r;
}

void Primitive::uvwToPosition(
    const Vector3R& uvw, 
    Vector3R* const out_position) const 
{
    CADISE_ASSERT(out_position);

    return;
}

const Bsdf* Primitive::bsdf() const 
{
    return _bsdf.get();
}

const AreaLight* Primitive::areaLight() const
{
    return _areaLight;
}

void Primitive::setAreaLight(const AreaLight* const areaLight)
{
    CADISE_ASSERT(areaLight);

    _areaLight = areaLight;
}

} // namespace cadise