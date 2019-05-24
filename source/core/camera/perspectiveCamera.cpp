#include "core/camera/perspectiveCamera.h"

#include "core/ray.h"

#include "math/constant.h"

#include <limits>

// Tmp
#include <random>

namespace cadise {

PerspectiveCamera::PerspectiveCamera() {
}

PerspectiveCamera::PerspectiveCamera(Matrix4 cameraToWorld, float fov, Path filename, int rx, int ry) :
    _cameraToWorld(cameraToWorld), _fov(fov), _rx(rx), _ry(ry) {

    _film = Film(filename, _rx, _ry);

    float halfScreenLength = tanf(fov / 2.0f * constant::PI / 180.0f);
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
    float left = -tanf(_fov / 2.0f * constant::PI / 180.0f);
    float up = tanf(_fov / 2.0f * constant::PI / 180.0f);
    Vector3F samplePoint = Vector3F(left + sampleX, up - sampleY, -1.0f);

    Vector3F origin = _cameraToWorld.transformPoint(Vector3F(0.0f, 0.0f, 0.0f));
    Vector3F dir = _cameraToWorld.transformVector(samplePoint);

    // create ray in world space
    return Ray(origin, dir, constant::RAY_EPSILON, std::numeric_limits<float>::max());
}

Film PerspectiveCamera::film() {
    return _film;
}

} // namespace cadise