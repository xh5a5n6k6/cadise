#pragma once

#include "core/camera/camera.h"

#include "core/film.h"

#include "math/matrix.h"

namespace cadise {

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(Matrix4 cameraToWorld, float fov, Path filename, int rx, int ry);

    Ray createRay(int px, int py) override;

    Film film() override;

private:
    Film _film;
    Matrix4 _cameraToWorld;
    int _rx, _ry;
    float _fov;
    float _pixelWidth, _pixelHeight;
};

} // namespace cadise