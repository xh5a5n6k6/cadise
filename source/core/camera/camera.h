#pragma once

#include "math/vector.h"

namespace cadise {

class Ray;

class Camera {
public:
    Camera(const Vector3R& position);

    virtual ~Camera();

    virtual void updateTransform() = 0;
    virtual Ray spawnPrimaryRay(const Vector2R& filmNdcPosition) const = 0;

    void setAspectRatio(const real aspectRatio);

protected:
    Vector3R _position;
    real _aspectRatio;
};

} // namespace cadise