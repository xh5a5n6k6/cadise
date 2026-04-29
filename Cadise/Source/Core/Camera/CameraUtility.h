#pragma once

#include "Math/Type/MathType.h"

namespace cadise
{

class CameraUtility
{
public:
    /*! @brief Construct the view (LookAt) transform from the camera's data.

    The view transform is used to transform given point or vector
    between camera space and world space. More specifically,
    the return transform will transform input from camera space to world space.
    */
    static Transform4D buildLookAtTransform(
        const Vector3D& cameraPosition,
        const Vector3D& forwardDirection,
        const Vector3D& upDirection);
};

} // namespace cadise