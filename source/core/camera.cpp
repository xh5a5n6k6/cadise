#include "core/camera.h"

// Tmp
#include <random>

namespace cadise {

Camera::Camera() {
}

Camera::Camera(Matrix4 worldToCamera) :
    _worldToCamera(worldToCamera) {
}

PerspectiveCamera::PerspectiveCamera() {
}

PerspectiveCamera::PerspectiveCamera(int rx, int ry, Matrix4 worldToCamera, float fov) :
    _rx(rx), _ry(ry), Camera(worldToCamera), _fov(fov) {

    float halfScreenLength = tanf(fov / 2.0f * M_PI / 180.0f);
    _pixelWidth = 2.0f * halfScreenLength / _rx;
    _pixelHeight = 2.0f * halfScreenLength / _ry;
}

Ray PerspectiveCamera::createRay(int px, int py) {
    // TODO
    // improve sample point calculation
    std::random_device rd;
    std::default_random_engine gen = std::default_random_engine(rd());
    std::uniform_real_distribution<float> disX(0.0f, 1.0f);
    std::uniform_real_distribution<float> disY(0.0f, 1.0f);
    float sx = disX(gen);
    float sy = disY(gen);


    float sampleX = px * _pixelWidth + sx * _pixelWidth;
    float sampleY = py * _pixelHeight + sy * _pixelHeight;
    float left = -tanf(_fov / 2.0f * M_PI / 180.0f);
    float up = tanf(_fov / 2.0f * M_PI / 180.0f);
    Vector3 samplePoint = Vector3(left + sampleX, up - sampleY, -1.0f);

    // from camera space to world space
    Matrix4 cameraToWorld = _worldToCamera.inverse();

    Vector3 origin = TransformPoint(cameraToWorld, Vector3());
    Vector3 dir = TransformVector(cameraToWorld, samplePoint);

    // create ray in world space
    Ray ray = Ray(origin, dir, CADISE_RAY_EPSILON, FLT_MAX);

    return ray;
}

} // namespace cadise