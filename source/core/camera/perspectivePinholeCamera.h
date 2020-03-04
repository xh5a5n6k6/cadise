#pragma once

#include "core/camera/camera.h"

#include <memory>
#include <utility>

namespace cadise {

class Transform;

class PerspectivePinholeCamera : public Camera {
public:
    PerspectivePinholeCamera(
        const Vector3R& position, 
        const Vector3R& direction, 
        const Vector3R& up, 
        const real      fov,
        const real      sensorWidthMM);

    void updateTransform() override;
    Ray spawnPrimaryRay(const Vector2R& filmNdcPosition) const override;

    void evaluateCameraSample(
        CameraSample* const out_sample, 
        Ray* const          out_ray) const override;
    void evaluateCameraPdf(
        const Ray&  cameraRay, 
        real* const out_pdfA, 
        real* const out_pdfW) const override;

private:
    std::pair<real, real> _getSensorSize() const;

    real _getSensorArea() const;

    std::shared_ptr<Transform> _cameraToWorld;

    // transform from film NDC (not raster) to camera
    std::shared_ptr<Transform> _filmNdcToCamera;

    real _fov;
    real _sensorWidthMM;
    real _sensorOffset;
};

} // namespace cadise