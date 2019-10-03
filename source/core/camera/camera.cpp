#include "core/camera/camera.h"

namespace cadise {

Camera::Camera(const Vector3R& position) :
    _position(position),
    _aspectRatio(16.0_r / 9.0_r) {
}

void Camera::setAspectRatio(const real aspectRatio) {
    _aspectRatio = aspectRatio;
}

} // namespace cadise