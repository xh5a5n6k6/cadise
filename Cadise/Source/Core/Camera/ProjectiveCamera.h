#pragma once

#include "Core/Camera/Camera.h"
#include "Math/TVector2.h"

namespace cadise
{

class ProjectiveCamera : public Camera
{
public:
    ProjectiveCamera(
        const Transform4D& cameraToWorld,
        const Transform4D& filmToCamera,
        const Vector2S&    resolution);

    void spawnPrimaryRay(
        const Vector2D& positionRS,
        Ray* const      out_primaryRay) const override = 0;

    void evaluateCameraSample(
        CameraSample* const out_sample,
        Ray* const          out_toCameraRay) const override = 0;

    void evaluateCameraPdf(
        const Ray&  cameraRay,
        real* const out_pdfA,
        real* const out_pdfW) const override = 0;

protected:
    /*!
    Check if the position in raster space is inside this camera's view frustum.
    Because it's already in raster space, we only need to check x and y coordinates.
    */
    bool _isInsideViewFrustum(const Vector3D& positionRS) const;

    Transform4D _filmToCamera;
    Vector2S    _resolution;
};

} // namespace cadise