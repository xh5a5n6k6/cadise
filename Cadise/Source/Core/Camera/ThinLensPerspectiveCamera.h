#pragma once

#include "Core/Camera/Camera.h"

#include <memory>
#include <utility>

// forward declaration
namespace cadise
{
    class Transform;
}

namespace cadise 
{

class ThinLensPerspectiveCamera : public Camera
{
public:
    ThinLensPerspectiveCamera(
        const Vector3R& position, 
        const Vector3R& direction, 
        const Vector3R& up, 
        const float64   fov,
        const float64   sensorWidthMM,
        const float64   focalDistanceMM,
        const float64   lensRadiusMM);

    void updateTransform() override;

    void spawnPrimaryRay(
        const Vector2D& filmPosition,
        Ray* const      out_primaryRay) const override;

    void evaluateCameraSample(
        CameraSample* const out_sample, 
        Ray* const          out_toCameraRay) const override;

    void evaluateCameraPdf(
        const Ray&  cameraRay, 
        real* const out_pdfA, 
        real* const out_pdfW) const override;

private:
    std::pair<float64, float64> _getSensorSizeXy() const;
    float64 _getSensorArea() const;

    std::shared_ptr<Transform> _cameraToWorld;

    // transform from film (raster) to camera
    std::shared_ptr<Transform> _filmToCamera;

    float64 _fov;
    float64 _sensorWidthM;
    float64 _sensorOffset; // focal length
    float64 _focalDistanceM;
    float64 _lensRadiusM;

    // f-number = focal length / aperture diameter
    // aperture diameter = focal length / f-number
};

} // namespace cadise