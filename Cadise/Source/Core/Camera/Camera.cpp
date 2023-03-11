#include "Core/Camera/Camera.h"

namespace cadise 
{

Camera::Camera(const Vector3R& position) :
    _position(position),
    _resolution(0) 
{}

Camera::~Camera() = default;

void Camera::setResolution(const Vector2S& resolution) 
{
    _resolution = resolution;
}

float64 Camera::_getAspectRatio() const 
{
    return static_cast<float64>(_resolution.x()) / static_cast<float64>(_resolution.y());
}

} // namespace cadise