#pragma once

#include "Core/Camera/ProjectiveCamera.h"

namespace cadise
{

class ThinLensPerspectiveCamera : public ProjectiveCamera
{
public:
    ThinLensPerspectiveCamera(
        const Transform4D& cameraToWorld,
        const Transform4D& filmToCamera,
        const Vector2S&    resolution,
        const Vector2D&    sensorSizeM,
        const float64      sensorOffsetM,
        const float64      focalDistanceM,
        const float64      lensRadiusM);

    void spawnPrimaryRay(
        const Vector2D& positionRS,
        Ray* const      out_primaryRay) const override;

    void evaluateCameraSample(
        CameraSample* const out_sample,
        Ray* const          out_toCameraRay) const override;

    void evaluateCameraPdf(
        const Ray&  cameraRay,
        real* const out_pdfA,
        real* const out_pdfW) const override;

private:
    Vector2D _sensorSizeM; // x: width, y: height
    float64  _sensorOffsetM; // it also means focal length
    float64  _focalDistanceM;
    float64  _lensRadiusM;
};

} // namespace cadise