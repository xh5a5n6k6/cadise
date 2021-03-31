#pragma once

#include "math/tVector2.h"
#include "math/tVector3.h"

namespace cadise {

class CameraSample;
class Ray;

class Camera {
public:
    explicit Camera(const Vector3R& position);

    virtual ~Camera();

    virtual void updateTransform() = 0;

    virtual void spawnPrimaryRay(
        const Vector2D& filmPosition,
        Ray* const      out_primaryRay) const = 0;

    virtual void evaluateCameraSample(
        CameraSample* const out_sample, 
        Ray* const          out_toCameraRay) const = 0;

    virtual void evaluateCameraPdf(
        const Ray&  cameraRay,
        real* const out_pdfA,
        real* const out_pdfW) const = 0;

    void setResolution(const Vector2S& resolution);

protected:
    float64 _getAspectRatio() const;

    Vector3R _position;
    Vector2S _resolution;
};

} // namespace cadise