#pragma once

#include "core/camera/camera.h"

#include "math/matrix.h"

namespace cadise {

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera();
    PerspectiveCamera(Matrix4 cameraToWorld, int rx, int ry, float fov);

    Ray createRay(int px, int py) override;

private:
    //Image _image;
    Matrix4 _cameraToWorld;
    int _rx, _ry;
    float _fov;
    float _pixelWidth, _pixelHeight;
};

} // namespace cadise