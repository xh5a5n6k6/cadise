#include "core/intersector/primitive/primitive.h"

#include "core/bsdf/bsdf.h"
#include "core/light/areaLight.h"

namespace cadise {

Primitive::Primitive() = default;

Primitive::Primitive(const std::shared_ptr<Bsdf>& bsdf) :
    _bsdf(bsdf),
    _textureMapper(nullptr),
    _areaLight() {
}

std::shared_ptr<Bsdf> Primitive::bsdf() const {
    return _bsdf;
}

bool Primitive::isEmissive() const {
    const std::shared_ptr<AreaLight> areaLight = _areaLight.lock();
    return areaLight != nullptr;
}

Spectrum Primitive::emittance(const Vector3R& emitDirection, const SurfaceInfo& emitSurface) const {
    const std::shared_ptr<AreaLight> areaLight = _areaLight.lock();
    return areaLight->emittance(emitDirection, emitSurface);
}

void Primitive::setAreaLight(const std::shared_ptr<AreaLight>& areaLight) {
    _areaLight = areaLight;
}

} // namespace cadise