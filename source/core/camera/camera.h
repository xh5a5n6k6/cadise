#pragma once

#include "math/vector.h"

namespace cadise {

class CameraSample;
class Ray;

class Camera {
public:
    Camera(const Vector3R& position);

    virtual ~Camera();

    virtual void updateTransform() = 0;
    virtual Ray spawnPrimaryRay(const Vector2R& filmNdcPosition) const = 0;

    virtual void evaluateCameraSample(
        CameraSample* const out_sample, 
        Ray* const          out_ray) const = 0;
    virtual void evaluateCameraPdf(
        const Ray&  cameraRay,
        real* const out_pdfA,
        real* const out_pdfW) const = 0;

    void setAspectRatio(const real aspectRatio);

protected:
    Vector3R _position;
    real     _aspectRatio;
};

} // namespace cadise