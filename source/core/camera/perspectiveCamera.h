#pragma once

#include "core/camera/camera.h"

#include "core/film.h"

#include "math/matrix.h"

namespace cadise {

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(const Film& film, const real fov, const Vector3R* const lookAt);
    PerspectiveCamera(const Film& film, const real fov, const Matrix4& cameraToWorld);

    Ray createRay(const int32 px, const int32 py) const override;

    Film film() const override;

private:
    Film _film;
    real _fov;
    Matrix4 _cameraToWorld;

    real _pixelWidth;
    real _pixelHeight;
};

} // namespace cadise