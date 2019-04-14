#include "core/light/pointLight.h"

namespace cadise {

PointLight::PointLight(Vector3 position, Vector3 color) :
    _position(position), _color(color) {
}

Vector3 PointLight::position() {
    return _position;
}

Vector3 PointLight::color() {
    return _color;
}

} // namespace cadise