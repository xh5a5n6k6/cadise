#pragma once

#include "Math/TMatrix4.h"
#include "Math/TTransform.h"
#include "Math/TVector3.h"

namespace cadise
{

class CameraUtility
{
public:
    /*! @brief Build the view transform from camera vectors.

    The view transform, known as the LookAt transform, is used to
    transform given point or vector from camera space to world space.    
    */
    template<typename T>
    static TTransform<T> buildLookAtTransform(
        const TVector3<T>& cameraPosition,
        const TVector3<T>& forwardDirection,
        const TVector3<T>& upDirection);
};

// template header implementation

template<typename T>
inline TTransform<T> CameraUtility::buildLookAtTransform(
    const TVector3<T>& cameraPosition,
    const TVector3<T>& forwardDirection,
    const TVector3<T>& upDirection)
{
    const TVector3<T> newZ = forwardDirection.negate().normalize();
    const TVector3<T> newX = upDirection.cross(newZ).normalize();
    const TVector3<T> newY = newZ.cross(newX);

    const TMatrix4<T> cameraToWorld(
        newX.x(), newY.x(), newZ.x(), cameraPosition.x(),
        newX.y(), newY.y(), newZ.y(), cameraPosition.y(),
        newX.z(), newY.z(), newZ.z(), cameraPosition.z(),
        T(0),     T(0),     T(0),     T(1));

    return TTransform<T>(cameraToWorld);
}

} // namespace cadise