#pragma once

#include "math/algebra.h"
#include "core/ray.h"
#include "core/image.h"

namespace cadise {

class Camera {
public:
    Camera();
    Camera(Matrix4 worldToCamera);

    virtual Ray createRay(int px, int py) = 0;

    Matrix4 _worldToCamera;
};

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera();
    PerspectiveCamera(int rx, int ry, Matrix4 worldToCamera, float fov);

    Ray createRay(int px, int py) override;
	
    Image _image;
    int _rx, _ry;
    float _pixelWidth, _pixelHeight;
    float _fov;
}; 

} // namespace cadise