#include "core/intersector/primitive/primitive.h"

#include "core/bsdf/bsdf.h"
#include "core/light/category/areaLight.h"

namespace cadise {

Primitive::Primitive() = default;

Primitive::Primitive(const std::shared_ptr<BSDF>& bsdf) :
    _bsdf(bsdf),
    _textureMapper(nullptr),
    _areaLight() {
}

std::shared_ptr<BSDF> Primitive::bsdf() const {
    return _bsdf;
}

Spectrum Primitive::emittance(const Vector3R& outDirection) const {
    std::shared_ptr<AreaLight> areaLight = _areaLight.lock();
    return (areaLight == nullptr) ? Spectrum(0.0_r) : areaLight->color();
}

void Primitive::setAreaLight(const std::shared_ptr<AreaLight>& areaLight) {
    _areaLight = areaLight;
}

} // namespace cadise