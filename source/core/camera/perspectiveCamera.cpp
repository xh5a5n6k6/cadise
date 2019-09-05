#include "core/camera/perspectiveCamera.h"

#include "core/ray.h"

#include "math/constant.h"
#include "math/math.h"
#include "math/random.h"

#include <limits>

namespace cadise {

PerspectiveCamera::PerspectiveCamera(const Film& film, const real fov, const Vector3R* const lookAt) :
    _film(film),
    _fov(fov),
    _cameraToWorld(Matrix4::lookAt(lookAt[0], lookAt[1], lookAt[2])) {

    int32 rx = _film.resolution().x();
    int32 ry = _film.resolution().y();
    real aspectRatio = static_cast<real>(ry) / static_cast<real>(rx);

    real screenWidth = 2.0_r * std::tan(math::degreeToRadian(_fov / 2.0_r));
    real screenHeight = screenWidth * aspectRatio;
    _pixelWidth = screenWidth / rx;
    _pixelHeight = screenHeight / ry;
}

Ray PerspectiveCamera::spawnPrimaryRay(const int32 px, const int32 py) const {
    real sx = random::get1D();
    real sy = random::get1D();

    real sampleX = px * _pixelWidth + sx * _pixelWidth;
    real sampleY = py * _pixelHeight + sy * _pixelHeight;

    int32 rx = _film.resolution().x();
    int32 ry = _film.resolution().y();
    real aspectRatio = static_cast<real>(ry) / static_cast<real>(rx);
    real halfScreenWidth  = std::tan(math::degreeToRadian(_fov / 2.0_r));
    real halfScreenHeight = halfScreenWidth * aspectRatio;

    real left = -halfScreenWidth;
    real up = halfScreenHeight;
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