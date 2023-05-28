#include "Core/Camera/Camera.h"

namespace cadise
{

Camera::Camera(const Transform4D& cameraToWorld) :
    _cameraToWorld(cameraToWorld)
{}

Camera::~Camera() = default;

Vector3D Camera::_getWorldSpacePosition() const
{
    Vector3D position;
    _cameraToWorld.transformPoint({ 0, 0, 0 }, &position);

    return position;
}

} // namespace cadise