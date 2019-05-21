#include "core/camera/perspectiveCamera.h"

#include "core/ray.h"

#include "math/constant.h"

#include <limits>

// Tmp
#include <random>

namespace cadise {

PerspectiveCamera::PerspectiveCamera(Matrix4 cameraToWorld, int rx, int ry, float fov) :
    _cameraToWorld(cameraToWorld), _rx(rx), _ry(ry), _fov(fov) {

    float halfScreenLength = tanf(fov / 2.0f * CADISE_PI / 180.0f);
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
    float left = -tanf(_fov / 2.0f * CADISE_PI / 180.0f);
    float up = tanf(_fov / 2.0f * CADISE_PI / 180.0f);
    Vector3 samplePoint = Vector3(left + sampleX, up - sampleY, -1.0f);

    Vector3 origin = _cameraToWorld.transformPoint(Vector3(0.0f, 0.0f, 0.0f));
    Vector3 dir = _cameraToWorld.transformVector(samplePoint);

    // create ray in world space
    return Ray(origin, dir, CADISE_RAY_EPSILON, std::numeric_limits<float>::max());
}

} // namespace cadise