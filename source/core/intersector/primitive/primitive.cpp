#include "core/intersector/primitive/primitive.h"

#include "core/bsdf/bsdf.h"
#include "core/color.h"
#include "core/light/areaLight.h"

namespace cadise {

Primitive::Primitive() {

}

Primitive::Primitive(const std::shared_ptr<BSDF>& bsdf) :
    _bsdf(bsdf), 
    _areaLight() {
}

std::shared_ptr<BSDF> Primitive::bsdf() const {
    return _bsdf;
}

RGBColor Primitive::emittance(const Vector3R& outDirection) const {
    std::shared_ptr<AreaLight> areaLight = _areaLight.lock();
    return (areaLight == nullptr) ? RGBColor(0.0_r, 0.0_r, 0.0_r) : areaLight->color();
}

void Primitive::setAreaLight(const std::shared_ptr<AreaLight>& areaLight) {
    _areaLight = areaLight;
}

} // namespace cadise