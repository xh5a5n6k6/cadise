#pragma once

#include "core/camera/camera.h"

#include "core/film.h"

#include "math/matrix.h"

namespace cadise {

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(Matrix4 cameraToWorld, real fov, Path filename, int32 rx, int32 ry);

    Ray createRay(int32 px, int32 py) override;

    Film film() override;

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