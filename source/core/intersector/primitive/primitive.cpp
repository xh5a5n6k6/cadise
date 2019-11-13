#include "core/intersector/primitive/primitive.h"

#include "fundamental/assertion.h"

namespace cadise {

Primitive::Primitive() = default;

Primitive::Primitive(const std::shared_ptr<Bsdf>& bsdf) :
    _bsdf(bsdf),
    _textureMapper(nullptr),
    _areaLight(nullptr) {

    CADISE_ASSERT(bsdf);
}

const Bsdf* Primitive::bsdf() const {
    return _bsdf.get();
}

const AreaLight* Primitive::areaLight() const {
    return _areaLight;
}

void Primitive::setAreaLight(const AreaLight* const areaLight) {
    CADISE_ASSERT(areaLight);

    _areaLight = areaLight;
}

} // namespace cadise