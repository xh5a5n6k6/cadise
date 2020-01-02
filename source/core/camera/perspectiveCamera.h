#pragma once

#include "core/camera/camera.h"

#include <memory>

namespace cadise {

class Transform;

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(const Vector3R& position, 
                      const Vector3R& direction, 
                      const Vector3R& up, 
                      const real      fov,
                      const real      sensorWidthMM);

    void updateTransform() override;
    Ray spawnPrimaryRay(const Vector2R& filmNdcPosition) const override;

private:
    std::shared_ptr<Transform> _cameraToWorld;

    // transform from film NDC(instead raster) to camera
    std::shared_ptr<Transform> _filmToCamera;

    real _fov;
    real _sensorWidthMM;
};

} // namespace cadise