#include "core/camera/perspectiveCamera.h"

#include "core/ray.h"

#include "math/constant.h"

#include <limits>

// Tmp
#include <random>

namespace cadise {

PerspectiveCamera::PerspectiveCamera(const Film& film, const real fov, const Matrix4& cameraToWorld) :
    _film(film),
    _fov(fov), 
    _cameraToWorld(cameraToWorld) {

    real halfScreenLength = std::tan(_fov / 2.0_r * constant::PI / 180.0_r);
    _pixelWidth = 2.0_r * halfScreenLength / _film.resolution().x();
    _pixelHeight = 2.0_r * halfScreenLength / _film.resolution().y();
}

Ray PerspectiveCamera::createRay(const int32 px, const int32 py) const {
    // TODO
    // improve sample point calculation
    std::random_device rd;
    std::default_random_engine gen = std::default_random_engine(rd());
    std::uniform_real_distribution<real> disX(0.0_r, 1.0_r);
    std::uniform_real_distribution<real> disY(0.0_r, 1.0_r);
    real sx = disX(gen);
    real sy = disY(gen);


    real sampleX = px * _pixelWidth + sx * _pixelWidth;
    real sampleY = py * _pixelHeight + sy * _pixelHeight;
    real left = -std::tan(_fov / 2.0_r * constant::PI / 180.0_r);
    real up = std::tan(_fov / 2.0_r * constant::PI / 180.0_r);
    Vector3R samplePoint = Vector3R(left + sampleX, up - sampleY, -1.0_r);

    Vector3R origin = _cameraToWorld.transformPoint(Vector3R(0.0_r, 0.0_r, 0.0_r));
    Vector3R dir = _cameraToWorld.transformVector(samplePoint);

    // create ray in world space
    return Ray(origin, dir, constant::RAY_EPSILON, std::numeric_limits<real>::max());
}

Film PerspectiveCamera::film() const {
    return _film;
}

} // namespace cadise