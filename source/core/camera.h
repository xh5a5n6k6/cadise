#pragma once

#include "core/image.h"

#include "math/matrix.h"

namespace cadise {

class Ray;

class Camera {
public:
    Camera();
    Camera(Matrix4 cameraToWorld);

    virtual Ray createRay(int px, int py) = 0;

    Matrix4 _cameraToWorld;
};

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera();
    PerspectiveCamera(int rx, int ry, Matrix4 cameraToWorld, float fov);

    Ray createRay(int px, int py) override;
	
    Image _image;
    int _rx, _ry;
    float _pixelWidth, _pixelHeight;
    float _fov;
}; 

} // namespace cadise