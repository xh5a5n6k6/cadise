#pragma once

#include "core/camera/camera.h"

#include "core/film.h"

#include "math/matrix.h"

namespace cadise {

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(const Matrix4 cameraToWorld, const real fov, const Path filename, const int32 rx, const int32 ry);

    Ray createRay(const int32 px, const int32 py) const override;

    Film film() const override;

private:
    Film _film;
    Matrix4 _cameraToWorld;

    int32 _rx;
    int32 _ry;

    real _fov;
    real _pixelWidth;
    real _pixelHeight;
};

} // namespace cadise