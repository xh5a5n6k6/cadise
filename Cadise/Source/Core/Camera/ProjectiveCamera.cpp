#include "Core/Camera/ProjectiveCamera.h"

namespace cadise
{

ProjectiveCamera::ProjectiveCamera(
    const Transform4D& cameraToWorld,
    const Transform4D& filmToCamera,
    const Vector2S&    resolution) :

    Camera(cameraToWorld),
    _filmToCamera(filmToCamera),
    _resolution(resolution)
{}

bool ProjectiveCamera::_isInsideViewFrustum(const Vector3D& positionRS) const
{
    return
        positionRS.x() >= 0.0 &&
        positionRS.y() >= 0.0 &&
        positionRS.x() < static_cast<float64>(_resolution.x()) &&
        positionRS.y() < static_cast<float64>(_resolution.y());
}

} // namespace cadise