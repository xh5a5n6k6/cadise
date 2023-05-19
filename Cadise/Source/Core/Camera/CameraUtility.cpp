#include "Core/Camera/CameraUtility.h"

#include "Math/TMatrix4.h"
#include "Math/TTransform4.h"
#include "Math/TVector3.h"

namespace cadise
{

Transform4D CameraUtility::buildLookAtTransform(
    const Vector3D& cameraPosition,
    const Vector3D& forwardDirection,
    const Vector3D& upDirection)
{
    const Vector3D newZ = forwardDirection.negate().normalize();
    const Vector3D newX = upDirection.cross(newZ).normalize();
    const Vector3D newY = newZ.cross(newX);

    const Matrix4D cameraToWorld(
        newX.x(), newY.x(), newZ.x(), cameraPosition.x(),
        newX.y(), newY.y(), newZ.y(), cameraPosition.y(),
        newX.z(), newY.z(), newZ.z(), cameraPosition.z(),
        0,        0,        0,        1);

    return Transform4D(cameraToWorld, cameraToWorld.inverse());
}

} // namespace cadise